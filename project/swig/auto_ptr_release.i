namespace std {
  template<class T> class auto_ptr
  {
    public:
      auto_ptr();
      auto_ptr(T * t);
      auto_ptr(auto_ptr<T>&);
      T * operator-> () const;
      void reset(T * t);
  };
}


%define SWIG_AUTO_PTR_RELEASE_PROXY(TYPE, PROXYCLASS)
    
#if defined(SWIGCSHARP)

%typemap (ctype) std::auto_ptr<TYPE> "void *"
%typemap (imtype, out="IntPtr") std::auto_ptr<TYPE> "HandleRef"
%typemap (cstype) std::auto_ptr<TYPE> "PROXYCLASS"
%typemap (out) std::auto_ptr<TYPE> %{
  $result = (void *)$1.release();
%}
%typemap(csout, excode=SWIGEXCODE) std::auto_ptr<TYPE> {
    IntPtr cPtr = $imcall;
    PROXYCLASS ret = (cPtr == IntPtr.Zero) ? null : new PROXYCLASS(cPtr, true);$excode
    return ret;
  }

#elif defined(SWIGJAVA)
%typemap (ctype) std::auto_ptr<TYPE> "void *"
%typemap (jtype, out="jlong") std::auto_ptr<TYPE> "long"
%typemap (jstype) std::auto_ptr<TYPE> "PROXYCLASS"
%typemap (javaout) std::auto_ptr<TYPE> {
    long cPtr = $jnicall;
    PROXYCLASS ret = (cPtr == 0) ? null : new PROXYCLASS(cPtr, true);
    return ret;
}
%typemap(out) std::auto_ptr<TYPE> {
      *(TYPE **)&$result = $1.release();
}
#elif defined(SWIGPERL)

%typemap(out) std::auto_ptr<TYPE>
    "ST(argvi) = sv_newmortal();
     SWIG_MakePtr(ST(argvi++), (void *) $1.release(), $descriptor(TYPE *), $shadow|SWIG_POINTER_OWN);";

#elif defined(SWIGPYTHON) || defined(SWIGRUBY)

%typemap(out) std::auto_ptr<TYPE>
    "$result = SWIG_NewPointerObj((void *) $1.release(), $descriptor(TYPE *), SWIG_POINTER_OWN);"

#elif defined(SWIGPHP)

%typemap(out) std::auto_ptr<TYPE>
{
   // Release the auto_ptr and create a Zend resource.
   SWIG_SetPointerZval($result, (void *) $1.release(), $descriptor(TYPE *), SWIG_POINTER_OWN);
   // Now create the PHP object to contain the class to which the auto_ptr
   // was referencing.
   zval *obj, *_cPtr;
   MAKE_STD_ZVAL(obj);
   MAKE_STD_ZVAL(_cPtr);
   *_cPtr = *$result;
   INIT_ZVAL(*$result);
   // Lookup the Class Entry for this class.
   zend_class_entry *ce;
   zend_class_entry **pce;
   if (FAILURE == zend_lookup_class("TYPE",strlen("TYPE"), &pce TSRMLS_CC))
   {
       SWIG_PHP_Error(E_ERROR, "Unable to locate class entry for TYPE.");
   }

   ce = *pce;
   // Initialize it, attach the _cPtr  and return it.
   object_init_ex(obj, ce);
   add_property_zval(obj, "_cPtr",_cPtr);
   *$result = *obj;
}
#else
#error "Unsupported SWIG language for auto_ptr_release"
#endif

%template() std::auto_ptr<TYPE>;
%enddef
;

%define SWIG_AUTO_PTR_RELEASE(TYPE)
    SWIG_AUTO_PTR_RELEASE_PROXY(TYPE, TYPE)
%enddef

SWIG_AUTO_PTR_RELEASE(Foo)
    ;
%define SWIG_RELEASE_AUTO_PTR(RETURN_TYPE, METHOD_NAME, PROTO, ARGS)
    %extend {
    RETURN_TYPE * METHOD_NAME PROTO {
        std::auto_ptr<RETURN_TYPE> auto_result = self->METHOD_NAME ARGS;
        return auto_result.release();
    }
}
%enddef
