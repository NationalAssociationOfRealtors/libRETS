namespace boost {
  template<class T> class shared_ptr
  {
    public:
      shared_ptr();
      shared_ptr(T * t);
      T * operator-> () const;
      void reset(T * t);
  };
}


%define SWIG_SHARED_PTR_RELEASE_PROXY(TYPE, PROXYCLASS)
    
#if defined(SWIGCSHARP)

%typemap (ctype) boost::shared_ptr<TYPE> "void *"
%typemap (imtype, out="System.IntPtr") boost::shared_ptr<TYPE> "HandleRef"
%typemap (cstype) boost::shared_ptr<TYPE> "PROXYCLASS"
%typemap (out) boost::shared_ptr<TYPE> %{
  $result = (void *)&$1;
%}
%typemap(csout, excode=SWIGEXCODE) boost::shared_ptr<TYPE> {
    System.IntPtr cPtr = $imcall;
    PROXYCLASS ret = (cPtr == System.IntPtr.Zero) ? null : new PROXYCLASS(cPtr, true);$excode
    return ret;
  }

#else
#error "Unsupported SWIG language for shared_ptr_release"
#endif

%template() boost::shared_ptr<TYPE>;
%enddef
;

%define SWIG_SHARED_PTR_RELEASE(TYPE)
    SWIG_SHARED_PTR_RELEASE_PROXY(TYPE, TYPE)
%enddef

SWIG_SHARED_PTR_RELEASE(Foo)
    ;
