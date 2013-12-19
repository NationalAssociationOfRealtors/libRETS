/*
 * Copyright (C) 2005-2009 National Association of REALTORS(R)
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
#ifndef LIBRETS_METADATA_FOREIGN_KEY_H
#define LIBRETS_METADATA_FOREIGN_KEY_H
/** 
 * @file MetadataForeignKey.h
 * Contains the MetadataForeighKey class declaration.
 */

#include <string>
#include "librets/MetadataElement.h"

namespace librets {

/**
 * MetadataForeignKey is an implementation of MetadataElement that
 * represents Foreign Key information for the metadata
 */
class MetadataForeignKey : public MetadataElement
{
  public:
    virtual ~MetadataForeignKey();
    
    /**
     * Always returns FOREIGN_KEY.
     *
     * @return FOREIGN_KEY
     */
    virtual MetadataType GetType() const;
    
    /**
     * Return the Foreign Key ID.
     *
     * @return a string containing the Foreign Key ID.
     */
    std::string GetID() const;
     
    /**
     * Return the Foreign Key ID.
     *
     * @return a string containing the Foreign Key ID.
     */
    std::string GetForeignKeyID() const;

    /**
     * Return the Parent Resource ID.
     *
     * @return a string containing the Parent Resource ID.
     */
    std::string GetParentResourceID() const;
      
    /**
     * Return the Parent Class ID.
     *
     * @return a string containing the Parent Class ID.
     */
    std::string GetParentClassID() const;
      
    /**
     * Return the Parent System Name.
     *
     * @return a string containing the Parent System Name.
     */
    std::string GetParentSystemName() const;
     
    /**
     * Return the Child Resource ID.
     *
     * @return a string containing the Child Resource ID.
     */
    std::string GetChildResourceID() const;
     
    /**
     * Return the Child Class ID.
     * 
     * @return a string containing the Child Class ID.
     */
    std::string GetChildClassID() const;
     
    /**
     * Return the Child System Name.
     *
     * @return a string containing the Child System Name.
     */
    std::string GetChildSystemName() const;
    
    /**
     * Return the Conditional Parent Field (RETS 1.7 and later)
     *
     * @return a string containing the Conditional Parent Field.
     */
    std::string GetConditionalParentField() const;
    
    /**
     * Return the Conditional Parent Value (RETS 1.7 and later)
     *
     * @return a string containing the Conditional Parent Value.
     */
    std::string GetConditionalParentValue() const;
    
    /**
     * RETS 1.8
     * Return the OneToMany flag
     *
     * @return if <code>true</code> indicates that the foreign key
     * will return multiple rows.
     */
    bool GetOneToManyFlag() const;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */
