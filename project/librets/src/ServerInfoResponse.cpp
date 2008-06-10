/*
 * Copyright (C) 2008 National Association of REALTORS(R)
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
#include "librets/CurlStream.h"
#include "librets/ServerInfoResponse.h"
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

ServerInfoResponse::ServerInfoResponse() 
            : mParseInputStream()
{
    mColumns.reset(new StringVector());
    mCount = -1;
    mEncoding = RETS_XML_DEFAULT_ENCODING;
    mMaxRows = false;
    mReplyCode = 0;
    mReplyText.clear();
}

ServerInfoResponse::~ServerInfoResponse()
{
}

void ServerInfoResponse::FixCompactArray(StringVector & compactArray,
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

void ServerInfoResponse::Parse(istreamPtr inputStream)
{
    SetInputStream(inputStream);
}

bool ServerInfoResponse::Parse()
{
    bool retval = false;
    
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
                string extendedMeaning;
                /*
                 * There can be an optional text event as the next event, so we
                 * try this in a try/catch block.
                 */
                try
                {
                    RetsXmlTextEventPtr textEvent =
                        mXmlParser->AssertNextIsTextEvent();
                    extendedMeaning = textEvent->GetText();
                }
                catch (RetsException e)
                {
                    throw RetsReplyException(replyCode, meaning);
                }
                /*
                 * If we're here, we have an extended message. It could contain one
                 * or more newlines, so remove them.
                 */
                ba::erase_all(extendedMeaning, "\n");
                throw RetsReplyException(replyCode, meaning, extendedMeaning);
            }
            /*
             * Beginning a transaction. Clear mMaxRows, mReplyCode and mReplyText.
             */
            mMaxRows = false;
            mReplyCode = 0;
            mReplyText.clear();
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
            ba::split(columns, text, ba::is_any_of(mDelimiter));
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
            ba::split(*data, text, ba::is_any_of(mDelimiter));
            FixCompactArray(*data, "data");
            
            mRows.push_back(data);
            mXmlParser->AssertNextIsEndEvent();
            /*
             * Once a single data row has been parsed, return to the caller.
             */
            retval = true;
            break;
        }
        else if (name == "DELIMITER")
        {
            istringstream hexString(startEvent->GetAttributeValue("value"));
            // Must go into an int, not a char, due to the special handling of
            // chars in istringstream
            int delimiterChar;
            hexString >> std::hex >> delimiterChar;
            mDelimiter.clear();
            mDelimiter += (char) delimiterChar;
        }
        else if (name == "MAXROWS")
        {
            mMaxRows = true;
        }
        else if (name == "RETS-STATUS")
        {
            istringstream replyCodeString(
                startEvent->GetAttributeValue("ReplyCode"));
            replyCodeString >> mReplyCode;
            mReplyText.clear();
            if (mReplyCode != 0)
            {
                mReplyText = startEvent->GetAttributeValue("ReplyText");
            }
        }
    }
    
    return retval;
}

bool ServerInfoResponse::HasNext()
{
    /*
     * We are in streaming mode. We need to fetch a row until we either successfully
     * get a row, or run out of data to parse.
     */
    while (mNextRow == mRows.end() && Parse());
    if (mNextRow == mRows.end())
    {
        mCurrentRow.reset();
        return false;
    }
    /*
     * Since the parsing returns after parsing a single <DATA/> tag, the current
     * row will always be the one prior to the end of mRows.
     */
    mCurrentRow = *(mRows.end() - 1);
    mNextRow = mRows.end();
    return true;
}

int ServerInfoResponse::GetCount()
{
    return mCount;
}

const StringVector ServerInfoResponse::GetColumns()
{
    return *mColumns;
}

string ServerInfoResponse::GetString(int columnIndex)
{
    return mCurrentRow->at(columnIndex);
}

string ServerInfoResponse::GetString(string columnName)
{
    ColumnToIndexMap::const_iterator i = mColumnToIndex.find(columnName);
    if (i == mColumnToIndex.end())
    {
        throw invalid_argument("Invalid columnName: " + columnName);
    }
    return GetString(i->second);
}

void ServerInfoResponse::SetEncoding(EncodingType encoding)
{
    mEncoding = encoding;
}

EncodingType ServerInfoResponse::GetEncoding()
{
    return mEncoding;
}

int ServerInfoResponse::GetReplyCode()
{
    return mReplyCode;
}

string ServerInfoResponse::GetReplyText()
{
    return mReplyText;
}

void ServerInfoResponse::SetInputStream(istreamPtr inputStream)
{
    /*
     * Initialize for streaming mode.
     */
    mParseInputStream =  inputStream;
    
    ExpatXmlParserPtr XmlParser( new ExpatXmlParser(inputStream,
                    (mEncoding == RETS_XML_ISO_ENCODING 
                        ? "iso-8859-1"    
                        : "US-ASCII")));
    mXmlParser = XmlParser;

    mDelimiter = "\t";
    /*
     * Perform the initial parse.
     */
    Parse();
    
    mNextRow = mRows.begin();
    mCurrentRow.reset();
}

