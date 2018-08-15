#ifndef SOPA_API_COMMON_H_
#define SOPA_API_COMMON_H_

#ifdef NDEBUG
#define cnml_assert(cond) do {} while (0);
#else
#define cnml_assert(cond) if (!(cond)) { \
    std::cout << "Error occur: " << __func__ << __LINE__; \
    exit(0); }
#endif

#define SOPA_TRY_CATCH(func)                \
try {                                       \
  func;                                     \
} catch (sopa::DomainError& d) {            \
  d.info();                                 \
  cnml_assert(0)                            \
  return CNML_STATUS_DOMAINERR;             \
} catch (sopa::InvalidArgument& i) {        \
  i.info();                                 \
  cnml_assert(0)                            \
  return CNML_STATUS_INVALIDARG;            \
} catch (sopa::LengthError& l) {            \
  l.info();                                 \
  cnml_assert(0)                            \
  return CNML_STATUS_LENGTHERR;             \
} catch (sopa::OutOfRange& o) {             \
  o.info();                                 \
  cnml_assert(0)                            \
  return CNML_STATUS_OUTOFRANGE;            \
} catch (sopa::RangeError& r) {             \
  r.info();                                 \
  cnml_assert(0)                            \
  return CNML_STATUS_RANGEERR;              \
} catch (sopa::OverflowError& o) {          \
  o.info();                                 \
  cnml_assert(0)                            \
  return CNML_STATUS_OVERFLOWERR;           \
} catch (sopa::UnderflowError& u) {         \
  u.info();                                 \
  cnml_assert(0)                            \
  return CNML_STATUS_UNDERFLOWERR;          \
} catch (sopa::ParameterCheckFailed& p) {   \
  p.info();                                 \
  cnml_assert(0)                            \
  return CNML_STATUS_INVALIDPARAM;          \
} catch (sopa::BadAlloc& b) {               \
  b.info();                                 \
  cnml_assert(0)                            \
  return CNML_STATUS_BADALLOC;              \
} catch (sopa::BadCast& b) {                \
  b.info();                                 \
  cnml_assert(0)                            \
  return CNML_STATUS_BADCAST;               \
} catch (sopa::RuntimeError& r) {           \
  r.info();                                 \
  cnml_assert(0)                            \
  return CNML_STATUS_NODEVICE;              \
}


#define CHECK_ENFORCE(condition, msg)                                         \
  do {                                                                        \
    if (!(condition)) {                                                       \
      sopa::ParameterCheckFailed p(msg);                                      \
      p.info();                                                               \
      cnml_assert(0)                                                          \
      return CNML_STATUS_INVALIDPARAM;                                        \
    }                                                                         \
  } while (false)

#endif
