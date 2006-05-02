/*
 * Copyright (C) 2005 National Association of REALTORS(R)
 *
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, and/or sell copies of the
 * Software, and to permit persons to whom the Software is furnished
 * to do so, provided that the above copyright notice(s) and this
 * permission notice appear in all copies of the Software and that
 * both the above copyright notice(s) and this permission notice
 * appear in supporting documentation.
 */

#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/version.hpp>
#include <stdexcept>
#include "librets/SearchResultSet.h"
#include "librets/ExpatXmlParser.h"
#include "librets/RetsXmlStartElementEvent.h"
#include "librets/RetsXmlEndElementEvent.h"
#include "librets/RetsXmlTextEvent.h"
#include "librets/RetsException.h"
#include "librets/RetsReplyException.h"
#include "librets/util.h"

using namespace librets;
using namespace librets::util;
using std::string;
using std::vector;
using std::istringstream;
using std::ostringstream;
using std::invalid_argument;
namespace b = boost;
namespace ba = boost::algorithm;

SearchResultSet::SearchResultSet()
{
    mColumns.reset(new StringVector());
    mCount = -1;
}

SearchResultSet::~SearchResultSet()
{
}

void SearchResultSet::FixCompactArray(StringVector & compactArray,
                                      std::string context)
{
    if (compactArray.size() < 2)
    {
        ostringstream message;
        message << "Unknown compact format: " << context << ": "
		        << Output(compactArray);
        throw RetsException(message.str());
    }

    if (!compactArray.front().empty())
    {
        ostringstream message;
        message << "First element should be empty: " << context << ": "
                << Output(compactArray);
        throw RetsException(message.str());
    }
    compactArray.erase(compactArray.begin());
    
#if BOOST_VERSION >= 103300
    if (!compactArray.back().empty())
    {
        ostringstream message;
        message << "Last element should be empty: " << context << ": "
                << Output(compactArray);
        throw RetsException(message.str());
    }
    compactArray.pop_back();
#endif
}

void SearchResultSet::Parse(istreamPtr inputStream)
{
    ExpatXmlParserPtr mXmlParser(new ExpatXmlParser(inputStream));
    RetsXmlStartElementEventPtr metadataEvent;
    string delimiter = "\t";
    while (mXmlParser->HasNext())
    {
        RetsXmlEventPtr event = mXmlParser->GetNextSkippingEmptyText();
        RetsXmlStartElementEventPtr startEvent
            = b::dynamic_pointer_cast<RetsXmlStartElementEvent>(event);
        if (!startEvent)
        {
            continue;
        }
        
        string name = startEvent->GetName();
        if (name == "RETS")
        {
            istringstream replyCodeString(
                startEvent->GetAttributeValue("ReplyCode"));
            int replyCode;
            replyCodeString >> replyCode;
            if (replyCode == 20201)
            {
                // No records found
                mCount = 0;
                continue;
            }
            else if (replyCode != 0)
            {
                string meaning = startEvent->GetAttributeValue("ReplyText");
                throw RetsReplyException(replyCode, meaning);
            }
        }
        else if (name == "COUNT")
        {
            istringstream count(startEvent->GetAttributeValue("Records"));
            count >> mCount;
        }
        else if (name == "COLUMNS")
        {
            RetsXmlTextEventPtr textEvent =
                mXmlParser->AssertNextIsTextEvent();
            string text = textEvent->GetText();
            StringVector columns;
            ba::split(columns, text, ba::is_any_of(delimiter));
            FixCompactArray(columns, "columns");

            for (StringVector::size_type i = 0; i < columns.size(); i++)
            {
                mColumns->push_back(columns.at(i));
                // Need to subtract 1, so it's zero-based
                mColumnToIndex[columns.at(i)] = i;
            }
            mXmlParser->AssertNextIsEndEvent();
        }
        else if (name == "DATA")
        {
            // AssertNextIsTextEvent() ignores empty text events. In this
            // case, an empty text event (one with just tabs) is valid.  So
            // here we use GetNextEvent() which does not ignore empty text
            // events.
            RetsXmlEventPtr xmlEvent = mXmlParser->GetNextEvent();
            RetsXmlTextEventPtr textEvent =
                mXmlParser->AssertTextEvent(xmlEvent, "DATA: ");
            string text = textEvent->GetText();
            StringVectorPtr data(new StringVector());
            ba::split(*data, text, ba::is_any_of(delimiter));
            FixCompactArray(*data, "data");
            
            mRows.push_back(data);
            mXmlParser->AssertNextIsEndEvent();
        }
        else if (name == "DELIMITER")
        {
            istringstream hexString(startEvent->GetAttributeValue("value"));
            // Must go into an int, not a char, due to the special handling of
            // chars in istringstream
            int delimiterChar;
            hexString >> std::hex >> delimiterChar;
            delimiter.clear();
            delimiter += (char) delimiterChar;
        }
    }
    mNextRow = mRows.begin();
    mCurrentRow.reset();
}

bool SearchResultSet::HasNext()
{
    if (mNextRow != mRows.end())
    {
        mCurrentRow = *mNextRow;
        mNextRow++;
        return true;
    }
    else
    {
        mCurrentRow.reset();
        return false;
    }
}

int SearchResultSet::GetCount()
{
    return mCount;
}

const StringVector SearchResultSet::GetColumns()
{
    return *mColumns;
}

string SearchResultSet::GetString(int columnIndex)
{
    return mCurrentRow->at(columnIndex);
}

string SearchResultSet::GetString(string columnName)
{
    ColumnToIndexMap::const_iterator i = mColumnToIndex.find(columnName);
    if (i == mColumnToIndex.end())
    {
        throw invalid_argument("Invalid columnName: " + columnName);
    }
    return GetString(i->second);
}
