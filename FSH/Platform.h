#ifdef ALWAYS_INLINE
#undef ALWAYS_INLINE
#endif
#define ALWAYS_INLINE [[gnu::always_inline]]

#ifdef FLATTEN
#undef FLATTEN
#endif
#define FLATTEN [[gnu::flatten]]

#ifdef NEVER_INLINE
#undef NEVER_INLINE
#endif
#define NEVERINLINE [[gnu::noinline]]
