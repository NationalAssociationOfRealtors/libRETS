/* -----------------------------------------------------------------------------
 * See the LICENSE file for information on copyright, usage and redistribution
 * of SWIG, and the README file for authors - http://www.swig.org/release.html.
 *
 * rubytracking.swg
 *
 * This file contains support for tracking mappings from 
 * Ruby objects to C++ objects.  This functionality is needed
 * to implement mark functions for Ruby's mark and sweep
 * garbage collector.
 * ----------------------------------------------------------------------------- */

#ifdef __cplusplus
extern "C" {
#endif

/* Ruby 1.8 actually assumes the first case. */
#if SIZEOF_VOIDP == SIZEOF_LONG
#  define SWIG2NUM(v) LONG2NUM((unsigned long)v)
#  define NUM2SWIG(x) (unsigned long)NUM2LONG(x)
#elif SIZEOF_VOIDP == SIZEOF_LONG_LONG
#  define SWIG2NUM(v) LL2NUM((unsigned long long)v)
#  define NUM2SWIG(x) (unsigned long long)NUM2LL(x)
#else
#  error sizeof(void*) is not the same as long or long long
#endif

#include "st.h"

/* Global Ruby hash table to store Trackings from C/C++
   structs to Ruby Objects. 
*/
static st_table *swig_ruby_trackings = NULL;

/* Setup a Ruby hash table to store Trackings */
SWIGRUNTIME void SWIG_RubyInitializeTrackings(void) {
  /* Create a hash table to store Trackings from C++ 
     objects to Ruby objects. */

  if ( !swig_ruby_trackings  )
      swig_ruby_trackings = (st_table *) st_init_numtable();
}

/* Get a Ruby number to reference an object */
SWIGRUNTIME VALUE SWIG_RubyObjectToReference(VALUE object) {
  /* We cast the object to an unsigned long
     and then store a reference to it using
     a Ruby number object. */

  /* Convert the Object to a Ruby number */
  return SWIG2NUM(object);
}

/* Get a Ruby object from a previously stored reference */
SWIGRUNTIME VALUE SWIG_RubyReferenceToObject(VALUE reference) {
  /* The provided Ruby number object is a reference
     to the Ruby object we want.*/

  /* Convert the Ruby number to a Ruby object */
  return NUM2SWIG(reference);
}

/* Add a Tracking from a C/C++ struct to a Ruby object */
SWIGRUNTIME void SWIG_RubyAddTracking(void* ptr, VALUE object) {
  /* In a Ruby hash table we store the pointer and
     the associated Ruby object.  The trick here is
     that we cannot store the Ruby object directly - if
     we do then it cannot be garbage collected.  So
     instead we typecast it as a unsigned long and
     convert it to a Ruby number object.*/

  /* Get a reference to the Ruby object as a Ruby number */
  VALUE value = SWIG_RubyObjectToReference(object);

  /* Store the mapping to the hash table. */
  st_insert (swig_ruby_trackings, (st_data_t) ptr, (st_data_t) value);
}

/* Get the Ruby object that owns the specified C/C++ struct */
SWIGRUNTIME VALUE SWIG_RubyInstanceFor(void* ptr) {
  /* Get a reference to the pointer as a Ruby number */
  VALUE value;

  /* Now lookup the value stored in the global hash table */
  if (!st_lookup (swig_ruby_trackings, (st_data_t) ptr, &value)) {
    return Qnil;
  }

  if (value == Qundef) {
    /* No object exists - return nil. */
    return Qnil;
  }
  else {
    /* Convert this value to Ruby object */
    return SWIG_RubyReferenceToObject(value);
  }
}

/* Remove a Tracking from a C/C++ struct to a Ruby object.  It
   is very important to remove objects once they are destroyed
   since the same memory address may be reused later to create
   a new object. */
SWIGRUNTIME void SWIG_RubyRemoveTracking(void* ptr) {
  /* Get a reference to the pointer as a Ruby number */

  /* Delete the object from the hash table by calling Ruby's
     do this we need to call the Hash.delete method.*/
  st_delete( swig_ruby_trackings, (st_data_t *) &ptr, NULL);
}

/* This is a helper method that unlinks a Ruby object from its
   underlying C++ object.  This is needed if the lifetime of the
   Ruby object is longer than the C++ object */
SWIGRUNTIME void SWIG_RubyUnlinkObjects(void* ptr) {
  VALUE object = SWIG_RubyInstanceFor(ptr);

  if (object != Qnil) {
    DATA_PTR(object) = 0;
  }
}


#ifdef __cplusplus
}
#endif
