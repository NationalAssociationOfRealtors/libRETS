/*
 * Copyright (C) 2009 National Association of REALTORS(R)
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
#include <boost/lexical_cast.hpp>
#include <boost/version.hpp>
#include <stdexcept>
#include "librets/CurlStream.h"
#include "librets/UpdateResponse.h"
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

UpdateResponse::UpdateResponse() 
            : mParseInputStream()
{
    mColumns.reset(new StringVector());
    mEncoding = RETS_XML_DEFAULT_ENCODING;
    mReplyCode = 0;
    mReplyText.clear();
}

UpdateResponse::~UpdateResponse()
{
}

void UpdateResponse::FixCompactArray(StringVector & compactArray,
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

const StringVector UpdateResponse::GetColumns()
{
    return *mColumns;
}

EncodingType UpdateResponse::GetEncoding()
{
    return mEncoding;
}

string UpdateResponse::GetErrorFieldName()
{
    return mCurrentError->at(0);
}

int UpdateResponse::GetErrorNumber()
{
    int retval;
    try
    {
        retval = boost::lexical_cast<int>(mCurrentError->at(1));
    }
    catch(boost::bad_lexical_cast &)
    {
        throw RetsException("Unable to parse error number");
    }
    return retval;
}

int UpdateResponse::GetErrorOffset()
{
    int retval;
    try
    {
        retval = boost::lexical_cast<int>(mCurrentError->at(2));
    }
    catch(boost::bad_lexical_cast &)
    {
        throw RetsException("Unable to parse error offset");
    }
    return retval;    
}

string UpdateResponse::GetErrorText()
{
    return mCurrentError->at(3);
}

int UpdateResponse::GetReplyCode()
{
    return mReplyCode;
}

string UpdateResponse::GetReplyText()
{
    return mReplyText;
}

string UpdateResponse::GetString(int columnIndex)
{
    return mCurrentRow->at(columnIndex);
}

string UpdateResponse::GetString(string columnName)
{
    ColumnToIndexMap::const_iterator i = mColumnToIndex.find(columnName);
    if (i == mColumnToIndex.end())
    {
        throw invalid_argument("Invalid columnName: " + columnName);
    }
    return GetString(i->second);
}

string UpdateResponse::GetWarningFieldName()
{
    return mCurrentWarning->at(0);
}

int UpdateResponse::GetWarningNumber()
{
    int retval;
    try
    {
        retval = boost::lexical_cast<int>(mCurrentWarning->at(1));
    }
    catch(boost::bad_lexical_cast &)
    {
        throw RetsException("Unable to parse Warning number");
    }
    return retval;
}

int UpdateResponse::GetWarningOffset()
{
    int retval;
    try
    {
        retval = boost::lexical_cast<int>(mCurrentWarning->at(2));
    }
    catch(boost::bad_lexical_cast &)
    {
        throw RetsException("Unable to parse Warning offset");
    }
    return retval;    
}

int UpdateResponse::GetWarningResponseRequired()
{
    int retval;
    try
    {
        retval = boost::lexical_cast<int>(mCurrentWarning->at(4));
    }
    catch(boost::bad_lexical_cast &)
    {
        throw RetsException("Unable to parse Warning Response Required");
    }
    return retval;        
}

string UpdateResponse::GetWarningText()
{
    return mCurrentWarning->at(3);
}

bool UpdateResponse::HasNext()
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

bool UpdateResponse::HasNextError()
{
    if (mNextError != mErrors.end())
    {
        mCurrentError = *mNextError;
        mNextError++;
        return true;
    }
    else
    {
        mCurrentError.reset();
        return false;
    }
}

bool UpdateResponse::HasNextWarning()
{
    if (mNextWarning != mWarnings.end())
    {
        mCurrentWarning = *mNextWarning;
        mNextWarning++;
        return true;
    }
    else
    {
        mCurrentWarning.reset();
        return false;
    }
}


void UpdateResponse::Parse(istreamPtr inputStream)
{
    SetInputStream(inputStream);
}

bool UpdateResponse::Parse()
{
    bool errorBlockSeen     = false;
    bool warningBlockSeen   = false;
    bool retval             = false;
    
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
            if (replyCode != 0)
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
             * Beginning a transaction. Clear mReplyCode and mReplyText.
             */
            mReplyCode = 0;
            mReplyText.clear();
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
        else if (name == "ERRORBLOCK")
        {
            errorBlockSeen = true;
            continue;
        }
        else if (name == "ERRORDATA")
        {
            if (!errorBlockSeen)
                throw RetsException("Malformed ERRORBLOCK.");
            
            // AssertNextIsTextEvent() ignores empty text events. In this
            // case, an empty text event (one with just tabs) is valid.  So
            // here we use GetNextEvent() which does not ignore empty text
            // events.
            RetsXmlEventPtr xmlEvent = mXmlParser->GetNextEvent();
            RetsXmlTextEventPtr textEvent =
                mXmlParser->AssertTextEvent(xmlEvent, "ERRORBLOCK: ");
            string text = textEvent->GetText();
            StringVectorPtr data(new StringVector());
            ba::split(*data, text, ba::is_any_of(mDelimiter));
            FixCompactArray(*data, "errorblock");
            
            mErrors.push_back(data);
            mXmlParser->AssertNextIsEndEvent();
        }
        else if (name == "WARNINGBLOCK")
        {
            warningBlockSeen = true;
            continue;
        }
        else if (name == "WARNINGDATA")
        {
            if (!warningBlockSeen)
                throw RetsException("Malformed WARNINGBLOCK.");
            
            // AssertNextIsTextEvent() ignores empty text events. In this
            // case, an empty text event (one with just tabs) is valid.  So
            // here we use GetNextEvent() which does not ignore empty text
            // events.
            RetsXmlEventPtr xmlEvent = mXmlParser->GetNextEvent();
            RetsXmlTextEventPtr textEvent =
            mXmlParser->AssertTextEvent(xmlEvent, "WARNINGBLOCK: ");
            string text = textEvent->GetText();
            StringVectorPtr data(new StringVector());
            ba::split(*data, text, ba::is_any_of(mDelimiter));
            FixCompactArray(*data, "warningblock");
            
            mWarnings.push_back(data);
            mXmlParser->AssertNextIsEndEvent();
        }
    }
    
    return retval;
}

void UpdateResponse::SetEncoding(EncodingType encoding)
{
    mEncoding = encoding;
}

void UpdateResponse::SetInputStream(istreamPtr inputStream)
{
    /*
     * Initialize for streaming mode.
     */
    mParseInputStream =  inputStream;
    
    ExpatXmlParserPtr XmlParser( new ExpatXmlParser(inputStream,
                    (mEncoding == RETS_XML_ISO_ENCODING 
                        ? "iso-8859-1"    
                        : (mEncoding == RETS_XML_UTF8_ENCODING
                            ? "UTF-8"
                            : "US-ASCII"))));
    mXmlParser = XmlParser;

    /*
     * Perform the parse. Doing this in a loop will cause the entire
     * transaction to be completed at once instead of relying upon the
     * "just-in-time" feature. If we want to implement that in the future
     * we must handle the changing of parse state from parsing DATA to 
     * parsing ERRORS to parsing WARNINGS.
     */
    while (Parse());
    
    mNextRow        = mRows.begin();
    mNextError      = mErrors.begin();
    mNextWarning    = mWarnings.begin();
    mCurrentError.reset();
    mCurrentRow.reset();
    mCurrentWarning.reset();
}

