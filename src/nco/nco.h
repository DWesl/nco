/* $Header: /data/zender/nco_20150216/nco/src/nco/nco.h,v 1.285 2013-03-05 08:36:08 pvicente Exp $ */

/* Purpose: netCDF Operator (NCO) definitions */

/* Copyright (C) 1995--2013 Charlie Zender
   License: GNU General Public License (GPL) Version 3
   See http://www.gnu.org/copyleft/gpl.html for full license text */

/* Usage:
   #include "nco.h" *//* netCDF Operator (NCO) definitions */

#ifndef NCO_H /* Contents have not yet been inserted in current source file */
#define NCO_H

/* Standard header files */
#include <stdio.h> /* stderr, FILE, NULL, printf */

/* 3rd party vendors */
#include <netcdf.h> /* netCDF definitions and C library */

/* Personal headers */
#include "nco_typ.h" /* Type definitions, opaque types */

  /* C pre-processor macros for instantiating variable values with string tokens
     Macros for token pasting described at http://www.parashift.com/c++-faq-lite
     Layer of indirection is required, use public macro to call private macro */
#define TKN2SNG_PRV(x) #x
#define TKN2SNG(x) TKN2SNG_PRV(x)

/* Encapsulate C++ const usage in C99-safe macro 
   C++ compilers will use type-safe version
   C89 and C99 compilers use less type-safe version that is standards-compliant */
#ifdef __cplusplus
# define CST_X_PTR_CST_PTR_CST_Y(x,y) const x * const * const y
# define X_CST_PTR_CST_PTR_Y(x,y) x const * const * y
#else /* !__cplusplus */
# define CST_X_PTR_CST_PTR_CST_Y(x,y) x * const * const y
# define X_CST_PTR_CST_PTR_Y(x,y) x * const * y
#endif /* !__cplusplus */

#ifdef __cplusplus
/* Use C-bindings so C++-compiled and C-compiled libraries are compatible */
extern "C" {
#endif /* !__cplusplus */

/* Replace restrict by __restrict in g++ compiler
   Substitute whitespace for restrict in all other C++ compilers */
#ifdef __cplusplus
# ifdef __GNUG__
#  define restrict __restrict
# else /* !__GNUG__ */
#  define restrict
# endif /* !__GNUG__ */
#endif /* !__cplusplus */

/* Boolean values */
/* From Wikipedia:
   "On a recent C compiler (supporting the C99 standard), there is a _Bool type, which is used to define bool by the stdbool.h header: 

   #include <stdbool.h>
   bool b = false;
   ...
   b = true;

   During its standardization process, the C++ programming language introduced the bool, true and false keywords, adding a native datatype to support boolean data.
   Preprocessor macros may be used to turn bool into _Bool, false into 0 and true into 1, allowing compatibility with the aforementioned C99 use of the stdbool.h header." */
#define nco_bool int
#ifndef __cplusplus
# ifndef bool
#  define bool int
# endif /* bool */
#endif /* __cplusplus */
#ifndef True
# define True 1
#endif /* True */
#ifndef False
# define False 0
#endif /* False */
  
  /* Variables marked CEWI "Compiler Error Warning Initialization" are initialized
     to prevent spurious "warning: `float foo' might be used uninitialized in 
     this function" warnings when, e.g., GCC -Wuninitialized is turned on.
     Note that these warning messages are compiler- and OS-dependent
     GCC warning on Alpha, e.g., cannot be removed by this trick */
#define NULL_CEWI NULL
#define char_CEWI '\0'
#define double_CEWI 0.0
#define float_CEWI 0.0
#define int_CEWI 0
#define long_CEWI 0L
#define long_long_CEWI ((long long int)0LL)
#define nco_bool_CEWI 0
#define nco_byte_CEWI ((signed char)0)
#define nco_char_CEWI '\0'
#define nco_int_CEWI (0L)
#define nco_short_CEWI ((short int)0)
#define nco_ubyte_CEWI ((unsigned char)0)
#define nco_ushort_CEWI ((unsigned short int)0)
#define nco_uint_CEWI (0U)
#define nco_int64_CEWI ((long long int)0LL)
#define nco_uint64_CEWI ((unsigned long long int)0ULL)
#define nco_string_CEWI NULL
#define short_CEWI 0
#define size_t_CEWI 0UL
  
  /* netcdf.h NC_GLOBAL is, strictly, the variable ID for global attributes
     NCO_REC_DMN_UNDEFINED is dimension ID of record dimension iff record dimension is undefined
     Normally using -1 for this ID is fine, but token makes meaning clearer
     NB: nc_inq() family returns -1 for missing record dimensions */
#define NCO_REC_DMN_UNDEFINED -1

  /* netcdf.h defines four NC_FORMAT tokens: NC_FORMAT_CLASSIC, ...
     The values are (currently) enumerated from one to four
     Operators need to check if fl_out_fmt has been user-specified
     Saftest way is to compare current value of fl_out_fmt to initial value 
     Initial value should be a number that will never be a true netCDF format */
#define NCO_FORMAT_UNDEFINED 0
  
  /* Argument to strtol() and strtoul() indicating base-10 conversions */
#define NCO_SNG_CNV_BASE10 10

  /* netCDF provides no guidance on maximum nesting of groups */
#define NCO_MAX_GRP_DEPTH 10

  /* Flag that iterator found no more groups in container */
#define NCO_LST_GRP 0

  /* Debugging level that quiets all non-requested informational messages
     This value is compared against user-selected dbg_lvl 
     Running operators with --quiet automatically sets dbg_lvl=NCO_DBG_QUIET */
#define NCO_DBG_QUIET 0

  /* NCO_MSS_VAL_SNG names attribute whose value is "skipped" by arithmetic, aka the missing value attribute
     Attribute name should be either "missing_value" or "_FillValue" */
#ifndef NCO_MSS_VAL_SNG
  /* 20070831: TKN2SNG() technique inserts quotes into string though same test code in c.c does not produce extra quotes. TODO nco905. */
  /*# define NCO_MSS_VAL_SNG missing_value */
  /*# define NCO_MSS_VAL_SNG _FillValue*/
  /*char nco_mss_val_sng[]=TKN2SNG(NCO_MSS_VAL_SNG);*/ /* [sng] Missing value attribute name */
  /* Arcane workaround using NCO_USE_FILL_VALUE necessary because TKN2SNG() macro above is broken. TODO nco905 */
# define NCO_USE_FILL_VALUE
# ifdef NCO_USE_FILL_VALUE
  /* NCO_NETCDF4_AND_FILLVALUE tells whether netCDF4 restrictions on 
     _FillValue operations (must be defined before variable written,
     cannot be changed after variable written) can affect output file */ 
#  ifdef ENABLE_NETCDF4
#   define NCO_NETCDF4_AND_FILLVALUE
#  endif /* !ENABLE_NETCDF4 */
# endif /* !NCO_USE_FILL_VALUE */
#endif /* NCO_MSS_VAL_SNG */

  /* Call assert() in form assert(expected==) */
#define NCO_SANITY_CHECK

  /* Prototype global functions before defining them in next block */
  char *nco_mss_val_sng_get(void); /* [sng] Missing value attribute name */
  char *nco_not_mss_val_sng_get(void); /* [sng] Not missing value attribute name */
  char *prg_nm_get(void);
  int prg_get(void);
  unsigned short dbg_lvl_get(void);

#ifdef MAIN_PROGRAM_FILE /* Current file contains main() */
  
  /* Tokens and variables with scope limited to main.c, and global variables allocated here */
  
  int prg; /* [enm] Program ID */
  int prg_get(void){return prg;} /* [enm] Program ID */
  
  char *prg_nm; /* [sng] Program name */
  char *prg_nm_get(void){return prg_nm;} /* [sng] Program name */
  
  unsigned short dbg_lvl=0; /* [enm] Debugging level */
  unsigned short dbg_lvl_get(void){return dbg_lvl;} /* [enm] Debugging level */

# ifdef NCO_USE_FILL_VALUE
  /* This arcane get()/set() usage necessary because TKN2SNG() macro above is broken. TODO nco905 */
  char nco_mss_val_sng[]="_FillValue"; /* [sng] Missing value attribute name */
  char nco_not_mss_val_sng[]="missing_value"; /* [sng] Not missing value attribute name */
# else /* !NCO_USE_FILL_VALUE */
  char nco_mss_val_sng[]="missing_value"; /* [sng] Missing value attribute name */
  char nco_not_mss_val_sng[]="_FillValue"; /* [sng] Not missing value attribute name */
# endif /* !NCO_USE_FILL_VALUE */
  char *nco_mss_val_sng_get(void){return nco_mss_val_sng;} /* [sng] Missing value attribute name */
  char *nco_not_mss_val_sng_get(void){return nco_not_mss_val_sng;} /* [sng] Not missing value attribute name */

#else /* MAIN_PROGRAM_FILE is NOT defined, i.e., current file does not contain main() */
  
  /* External references to global variables are declared as extern here
     Variables with local file scope in all files except main.c are allocated here */
  
#endif /* MAIN_PROGRAM_FILE is NOT defined, i.e., the current file does not contain main() */

/* Compatibility tokens for when NCO compiled with older netcdf.h
   It is hard to track where/when many tokens defined
   Easiest to individually check for pre-definition of each */
/* Three compatibility tokens new to netCDF4 netcdf.h: */
#ifndef NC_NETCDF4
# define NC_NETCDF4     (0x1000) /* Use netCDF-4/HDF5 format */
#endif
#ifndef NC_CHUNKED
# define NC_CHUNKED (0)
#endif
#ifndef NC_CONTIGUOUS
# define NC_CONTIGUOUS (1)
#endif
#ifndef NC_NOCHECKSUM
# define NC_NOCHECKSUM 0
#endif
#ifndef NC_FLETCHER32
# define NC_FLETCHER32 1
#endif

/* Six compatibility tokens not all available until netCDF 3.6.1 netcdf.h
   NC_64BIT_OFFSET is used (so far) only in nco_fl_utl.c */
#ifndef NC_CLASSIC_MODEL
# define NC_CLASSIC_MODEL 0x0100 /**< Enforce classic model. Mode flag for nc_create(). */
#endif
#ifndef NC_64BIT_OFFSET
# define NC_64BIT_OFFSET 0x0200 /* Use large (64-bit) file offsets */
#endif
#ifndef NC_DISKLESS
# define NC_DISKLESS 0x0008 /* Use diskless file. Mode flag for nc_open() or nc_create(). */
#endif
#ifndef NC_FORMAT_CLASSIC
# define NC_FORMAT_CLASSIC (1)
#endif
#ifndef NC_FORMAT_64BIT
# define NC_FORMAT_64BIT   (2)
#endif
#ifndef NC_FORMAT_NETCDF4
# define NC_FORMAT_NETCDF4 (3)
#endif
#ifndef NC_FORMAT_NETCDF4_CLASSIC
# define NC_FORMAT_NETCDF4_CLASSIC  (4) /* create netcdf-4 files, with NC_CLASSIC_MODEL. */
#endif

  /* Backwards-compatibility error codes for netCDF4
     Currently used only in nco_grp_utl.c */
#ifndef NC_EBADGRPID
# define NC_EBADGRPID (-116)    /**< Bad group ID. */
#endif

  /* Two backwards-compatibility error codes implemented in 201106 to diagnose problems with DAP
     Currently used only in nco_fl_utl.c */
#ifndef NC_ECANTREAD
# define NC_ECANTREAD (-102)    /**< Can't read. */
#endif
#ifndef NC_EDAPSVC
# define NC_EDAPSVC (-70)      /**< DAP server error */
#endif

  /* NCO began using MIN/MAX tokens in nco_pck.c on 20101130
     Not sure when these tokens were introduced to netcdf.h */
#ifndef NC_MAX_BYTE
# define NC_MAX_BYTE 127
#endif
#ifndef NC_MIN_BYTE
# define NC_MIN_BYTE (-NC_MAX_BYTE-1)
#endif
#ifndef NC_MAX_CHAR
# define NC_MAX_CHAR 255
#endif
#ifndef NC_MAX_SHORT
# define NC_MAX_SHORT 32767
#endif
#ifndef NC_MIN_SHORT
# define NC_MIN_SHORT (-NC_MAX_SHORT - 1)
#endif
#ifndef NC_MAX_INT
# define NC_MAX_INT 2147483647
#endif
#ifndef NC_MIN_INT
# define NC_MIN_INT (-NC_MAX_INT - 1)
#endif
#ifndef NC_MAX_FLOAT
# define NC_MAX_FLOAT 3.402823466e+38f
#endif
#ifndef NC_MIN_FLOAT
# define NC_MIN_FLOAT (-NC_MAX_FLOAT)
#endif
#ifndef NC_MAX_DOUBLE
# define NC_MAX_DOUBLE 1.7976931348623157e+308 
#endif
#ifndef NC_MIN_DOUBLE
# define NC_MIN_DOUBLE (-NC_MAX_DOUBLE)
#endif
#ifndef NC_MAX_UBYTE
# define NC_MAX_UBYTE NC_MAX_CHAR
#endif
#ifndef NC_MAX_USHORT
# define NC_MAX_USHORT 65535U
#endif
#ifndef NC_MAX_UINT
# define NC_MAX_UINT 4294967295U
#endif
#ifndef NC_MAX_INT64
# define NC_MAX_INT64 (9223372036854775807LL)
#endif
#ifndef NC_MIN_INT64
# define NC_MIN_INT64 (-9223372036854775807LL-1)
#endif
#ifndef NC_MAX_UINT64
# define NC_MAX_UINT64 (18446744073709551615ULL)
#endif
/* Endif older netcdf.h */

  /* Define compatibility tokens when user does not have znetCDF */
#ifndef ENABLE_ZNETCDF
# ifndef NC_COMPRESS
#  define NC_COMPRESS 0x200 /* bn file is compressed */
# endif
#endif /* !ENABLE_ZNETCDF */

/* Internal NCO return code indicating success */
#define NCO_NOERR 1
/* Internal NCO return code indicating failure */
#define NCO_ERR 0

  /* NB: Use EXIT_SUCCESS and EXIT_FAILURE as return codes to shell (e.g., Bash, Csh), not to other functions */
#ifndef EXIT_SUCCESS /* Most likely this is a SUN4 machine */
# define EXIT_SUCCESS 0
#endif /* SUN4 */
#ifndef EXIT_FAILURE /* Most likely this is a SUN4 machine */
# define EXIT_FAILURE 1
#endif /* SUN4 */

  enum prg{ /* [enm] Key value for all netCDF operators */
    ncap,
    ncatted,
    ncbo,
    ncea,
    ncecat,
    ncflint,
    ncks,
    ncpdq,
    ncra,
    ncrcat,
    ncrename,
    ncwa
  }; /* end prg enum */
  
  enum nco_dbg_typ_enm{ /* [enm] Debugging levels */
    /* List in increasing levels of verbosity */
    nco_dbg_quiet, /* 0 [enm] Quiet all non-error messages */
    nco_dbg_std,   /* 1 [enm] Standard mode. Minimal, but some, messages */
    nco_dbg_fl,    /* 2 [enm] Filenames */
    nco_dbg_scl,   /* 3 [enm] Scalars, other per-file information  */
    nco_dbg_grp,   /* 4 [enm] Groups, highest level per-file loop information */
    nco_dbg_var,   /* 5 [enm] Variables, highest level per-group loop information */
    nco_dbg_crr,   /* 6 [enm] Current task */
    nco_dbg_sbr,   /* 7 [enm] Subroutine names on entry and exit */
    nco_dbg_io,    /* 8 [enm] Subroutine I/O */
    nco_dbg_vec,   /* 9 [enm] Entire vectors */
    nco_dbg_vrb,   /* 10 [enm] Verbose, print everything possible */
    nco_dbg_old,   /* 11 [enm] Old debugging blocks not used anymore */
    nco_dbg_dev,   /* 12 [enm] NCO developer information; only useful for debugging */
    nco_dbg_nbr    /* 13 [enm] Number of debugging types (equals last enumerated value) */
  }; /* end nco_dbg_typ_enm */

  enum nco_op_typ{ /* [enm] Operation type */
    /* Types used in ncbo(): */
    nco_op_add, /* [enm] Add file_1 to file_2 */
    nco_op_dvd, /* [enm] Divide file_1 by file_2 */
    nco_op_mlt, /* [enm] Multiply file_1 by file_2 */
    nco_op_sbt, /* [enm] Subtract file_2 from file_1 */
    /* Types used in ncra(), ncrcat(), ncwa(): */
    nco_op_avg, /* [enm] Average */
    nco_op_min, /* [enm] Minimum value */
    nco_op_max, /* [enm] Maximum value */
    nco_op_ttl, /* [enm] Linear sum */
    nco_op_sqravg, /* [enm] Square of mean */
    nco_op_avgsqr, /* [enm] Mean of sum of squares */
    nco_op_sqrt, /* [enm] Square root of mean */
    nco_op_rms, /* [enm] Root-mean-square (normalized by N) */
    nco_op_rmssdn, /* [enm] Root-mean square normalized by N-1 */
    nco_op_nil /* [enm] Nil or undefined operation type */
  }; /* end nco_op_typ enum */

  enum nco_rlt_opr{ /* [enm] Arithmetic relations (comparisons) for masking */
    nco_op_eq, /* Equality */
    nco_op_ne, /* Inequality */
    nco_op_lt, /* Less than */
    nco_op_gt, /* Greater than */
    nco_op_le, /* Less than or equal to */
    nco_op_ge /* Greater than or equal to */
  }; /* end nco_rlt_opr enum */
  
  typedef enum nco_obj_typ_enm{ /* [enm] netCDF4 object type: group, variable */
    nco_obj_typ_err=-1, /* -1 Invalid type for initialization */
    nco_obj_typ_grp,    /*  0, Group */
    nco_obj_typ_var,    /*  1, Variable of atomic type */
    nco_obj_typ_nonatomic_var /*  2, Variable of non-atomic type (vlen, opaque, enum, compound, user-defined) */
  } nco_obj_typ; 
  
  typedef enum aed{ /* [enm] Attribute editor mode */
    aed_append,
    aed_create,
    aed_delete,
    aed_modify,
    aed_overwrite
  } aed_enm; /* end aed enum */
  
  typedef enum gpe{ /* [enm] Group Path Editing mode */
    gpe_append,
    gpe_delete,
    gpe_flatten,
    gpe_backspace
  } gpe_enm; /* end gpe enum */

  typedef enum { /* [enm] Memory allocation type */
    nco_mmr_calloc, /* [enm] nco_calloc() */
    nco_mmr_free, /* [enm] nco_free() */
    nco_mmr_malloc, /* [enm] nco_malloc() */
    nco_mmr_realloc /* [enm] nco_realloc() */
  } nco_mmr_typ_enm; /* end nco_mmr_typ enum */

  typedef enum { /* [enm] Arithmetic precision rank */
    /* Ranked ordering of "arithmetic precision" from lowest to highest */
    nco_rth_prc_rnk_string, /* [enm] */ /* Least precise */
    nco_rth_prc_rnk_char, /* [enm] */
    nco_rth_prc_rnk_ubyte, /* [enm] */
    nco_rth_prc_rnk_byte, /* [enm] */
    nco_rth_prc_rnk_ushort, /* [enm] */
    nco_rth_prc_rnk_short, /* [enm] */
    nco_rth_prc_rnk_uint, /* [enm] */
    nco_rth_prc_rnk_int, /* [enm] */
    nco_rth_prc_rnk_uint64, /* [enm] */
    nco_rth_prc_rnk_int64, /* [enm] */
    nco_rth_prc_rnk_float, /* [enm] */
    nco_rth_prc_rnk_double /* [enm] */ /* Most precise */
  } nco_rth_prc_rnk_enm; /* end nco_rth_prc_rnk enum */

  typedef enum { /* [enm] Timer flag */
    nco_tmr_srt, /* [enm] Initialize timer (first timer call) */
    nco_tmr_mtd, /* [enm] Metadata timer (second timer call) */
    nco_tmr_rgl, /* [enm] Regular timer call (main loop timer call) */
    nco_tmr_end /* [enm] Close timer (last timer call) */
  } nco_tmr_flg; /* [enm] Timer flag */

  typedef enum { /* [enm] Node enumerator Nie02 nodeEnum */
    typ_scv, /* [enm] Scalar value */
    typ_sym, /* [enm] Symbol identifier */
    typ_opr /* [enm] Operator */
  } nod_typ_enm; /* end Node enumerator */
  /* end enumeration section */

  typedef enum { /* [enm] Calendar type */
    cln_std=1, /* Standard mixed Julian/Gregorian */
    cln_grg, /* Gregorian Calendar */ 
    cln_jul, /* Julian Calendar */ 
    cln_360 , /* 30 days a month Calendar */ 
    cln_365, /* No Leap year Calendar */ 
    cln_366, /* Leap year Calendar */ 
    cln_nil /* No calendar found */
  } nco_cln_typ; /* [enm] Calendar type */
  
  /* Limit structure */
  typedef struct { /* lmt_sct */
    /* Following four flags are used only by multi-file operators ncra and ncrcat: */
    char *drn_sng; /* User-specified string for dimension duration */
    char *max_sng; /* User-specified string for dimension maximum */
    char *min_sng; /* User-specified string for dimension minimum */
    char *mro_sng; /* User-specified string for multi-record output */
    char *nm; /* [sng] Dimension name */
    char *rbs_sng; /* Used by ncra, ncrcat to re-base record coordinate (holds unit attribute from first file) */
    char *srd_sng; /* User-specified string for dimension stride */
    double max_val; /* Double precision representation of maximum value of coordinate requested or implied */
    double min_val; /* Double precision representation of minimum value of coordinate requested or implied */
    double origin;   /* Used by ncra, ncrcat to re-base record coordinate */
    int id; /* Dimension ID */
    int lmt_typ; /* crd_val or dmn_idx */
    long cnt; /* # of valid elements in this dimension (including effects of stride and wrapping) */
    long drn; /* Duration of hyperslab */
    long end; /* Index to end of hyperslab */
    long max_idx; /* Index of maximum requested value in dimension */
    long min_idx; /* Index of minimum requested value in dimension */
    long rec_dmn_sz; /* [nbr] Number of records in this file (multi-file record dimension only) */
    long rec_in_cml; /* [nbr] Cumulative number of records in all files opened so far (multi-file record dimension only) */
    long idx_end_max_abs; /* [idx] Maximum allowed index in record dimension (multi-file record dimension only) */
    long rec_skp_ntl_spf; /* [nbr] Records skipped in initial superfluous files (multi-file record dimension only) */
    long rec_skp_vld_prv; /* [nbr] Records skipped since previous good one (multi-file record dimension only) */
    long rec_rmn_prv_drn; /* [nbr] Records remaining-to-be-read to complete duration group from previous file (multi-file record dimension only) */
    long srd; /* Stride of hyperslab */
    long srt; /* Index to start of hyperslab */
    nco_bool flg_mro; /* True for multi-record output (used by ncra only) */
    nco_bool flg_input_complete; /* True for multi-file operators when no more files need be opened */
    nco_bool is_rec_dmn; /* True if record dimension, else False */
    nco_bool is_usr_spc_lmt; /* True if any part of limit is user-specified, else False */
    nco_bool is_usr_spc_max; /* True if user-specified, else False */
    nco_bool is_usr_spc_min; /* True if user-specified, else False */
    nco_cln_typ lmt_cln; /* Used by ncra, ncrcat to store enum of calendar-type attribute */
  } lmt_sct;
  
  /* Name ID structure */
  typedef struct{ /* nm_id_sct */
    char *grp_nm_fll; /* Fully qualified group where variable resides */
    char *var_nm_fll; /* Fully qualified variable name */
    char *nm; /* Variable (stub name) */
    int id; /* [id] Variable ID */
    int grp_id_in; /* [id] Group ID in input file */
    int grp_id_out; /* [id] Group ID in output file */
  } nm_id_sct;
  
  /* Rename structure */
  typedef struct{ /* rnm_sct */
    char *old_nm;
    char *new_nm;
    int id;
  } rnm_sct;

  /* DDRA information structure */
  typedef struct{ /* ddra_info_sct */
    int nco_op_typ; /* [enm] Operation type */
    int rnk_avg; /* [nbr] Rank of averaging space */
    int rnk_var; /* [nbr] Variable rank (in input file) */
    int rnk_wgt; /* [nbr] Rank of weight */
    int var_idx; /* [enm] Index */
    int wrd_sz; /* [B] Bytes per element */
    long long lmn_nbr; /* [nbr] Variable size */
    long long lmn_nbr_avg; /* [nbr] Averaging block size */
    long long lmn_nbr_wgt; /* [nbr] Weight size */
    nco_bool flg_ddra; /* [flg] Run DDRA diagnostics */
    nco_bool MRV_flg; /* [flg] Avergaging dimensions are MRV dimensions */
    nco_bool wgt_brd_flg; /* [flg] Broadcast weight for this variable */
    nco_tmr_flg tmr_flg; /* [enm] Timer flag */
  } ddra_info_sct;
  
  /* Pointer union */
  typedef union{ /* ptr_unn */
    float * restrict fp;
    double * restrict dp;
    nco_int * restrict ip;
    short * restrict sp;
    nco_char * restrict cp;
    nco_byte * restrict bp;
    nco_ubyte * restrict ubp;
    nco_ushort * restrict usp;
    nco_uint * restrict uip;
    nco_int64 * restrict i64p;
    nco_uint64 * restrict ui64p;
    nco_string * restrict sngp;
    void * restrict vp;
  } ptr_unn;

  /* Value union */
  typedef union{ /* val_unn */
    float f;
    double d;
    nco_int i;
    short s;
    nco_char c;
    nco_byte b;
    nco_ubyte ub;
    nco_ushort us;
    nco_uint ui;
    nco_int64 i64;
    nco_uint64 ui64;
    nco_string sng;
  } val_unn;

  /* Scalar value structure */
  typedef struct{ /* scv_sct */
    val_unn val; /* [sct] Value */
    nc_type type; /* [enm] netCDF type */
    nod_typ_enm nod_typ; /* [enm] Node type */
  } scv_sct;      

  /* Attribute editing structure */
  typedef struct{ /* aed_sct */
    char *att_nm; /* Name of attribute */
    char *var_nm; /* Name of variable, or NULL for global attribute */
    int id; /* Variable ID or NC_GLOBAL ( = -1) for global attribute */
    long sz; /* Number of elements in attribute */
    nc_type type; /* Type of attribute */
    ptr_unn val; /* Pointer to attribute value */
    aed_enm mode; /* Action to perform with attribute */
  } aed_sct;
  
  /* Attribute structure */
  typedef struct{ /* att_sct */
    char *nm;
    nc_type type;
    long sz;
    char fmt[5];
    ptr_unn val;
  } att_sct;
  
  /* Group Path Editing (GPE) structure */
  typedef struct{ /* gpe_sct */
    char *arg; /* [sng] Full GPE specification (for debugging) */
    char *edt; /* [sng] Editing component of full GPE specification */
    char *nm; /* [sng] Group name component of full GPE specification */
    char *nm_cnn; /* [sng] Canonicalized (i.e., slash-terminated) group name */
    gpe_enm md; /* [enm] Editing mode to perform */
    short lvl_nbr; /* [nbr] Number of levels to shift */
    size_t lng; /* [nbr] Length of user-specified group path */
    size_t lng_cnn; /* [nbr] Length of canonicalized user-specified group path */
    size_t lng_edt; /* [nbr] Length of editing component of full GPE specification */
  } gpe_sct;

 
  /* MSA Limits structure:
     GTT has a member for every unique dimension and for every coordinate variable */
  typedef struct { 
    char *dmn_nm;         /* [sng] Dimension name */
    long dmn_sz_org;      /* [nbr] Original size of dimension */
    long dmn_cnt;         /* [nbr] Hyperslabbed size of dimension */  
    nco_bool BASIC_DMN;   /* [flg] Limit is same as dimension in input file */
    nco_bool WRP;         /* [flg] Limit is wrapped (true if wrapping, lmt_dmn_nbr==2) */ 
    nco_bool MSA_USR_RDR; /* [flg] Multi-Slab Algorithm returns hyperslabs in user-specified order */
    int lmt_dmn_nbr;      /* [nbr] Number of lmt arguments */
    lmt_sct **lmt_dmn;    /* [sct] List of limit structures associated with each dimension */
    int lmt_crr;          /* [nbr] Index of current limit structure being initialized (helper to initialze lmt_sct*) */
  } lmt_msa_sct;

   /* GTT coordinate variable structure; it contains NetCDF model fields and a MSA field  */
  typedef struct{ 
    char *crd_nm_fll;       /* [sng] Full coordinate name */
    char *dmn_nm_fll;       /* [sng] Full name of dimension for *this* coordinate  */  
    char *crd_grp_nm_fll;   /* [sng] Full group name where coordinate is located */
    char *dmn_grp_nm_fll;   /* [sng] Full group name where dimension of *this* coordinate is located */
    char nm[NC_MAX_NAME+1L];/* [sng] Name of dimension and coordinate */
    nco_bool is_rec_dmn;    /* [flg] Is a record dimension? */
    size_t sz;              /* [nbr] Size of coordinate */
    nc_type var_typ;        /* [enm] NetCDF type  */  
    lmt_msa_sct lmt_msa;    /* [sct] MSA Limits structure for every coordinate */
  } crd_sct; 

   /* GTT dimension structure (stored in *groups*); it contains NetCDF model fields and a MSA field */
  typedef struct{ 
    char *grp_nm_fll;        /* [sng] Full group name where dimension was defined */
    char *nm_fll;            /* [sng] Dimension fully qualified name (path) */
    char nm[NC_MAX_NAME+1L]; /* [sng] Name of dimension (if coordinate variable, also name of variable) */
    nco_bool is_rec_dmn;     /* [flg] Is a record dimension? */
    size_t sz;               /* [nbr] Size of dimension */   
    int crd_nbr;             /* [nbr] Number of coordinate structures */
    crd_sct **crd;           /* [sct] List of coordinate structures associated with *this* dimension */
    lmt_msa_sct lmt_msa;     /* [sct] MSA Limits structure (implicit that is for non-coordinate case) */
    int id;                  /* [ID] Unique ID for dimension; same as "var_dmn_sct.id", obtained from API "nc_inq_dimid" */
  } dmn_trv_sct; 


  /* GTT Variable dimensions:
     A dimension has a name and a size, but it can have an associated variable (coordinate variable) */
  typedef struct{ 
    char *dmn_nm_fll; /* [sng] Full dimension name  */
    char *dmn_nm; /* [sng] Dimension name */
    char *grp_nm_fll; /* [sng] Full group where dimension is located  */   
    nco_bool is_crd_var; /* [flg] Is this *name* a coordinate variable or just a *non coordinate* dimension? */
    crd_sct *crd; /* [sct] Pointer to *coordinate variable* if coordinate variable */
    dmn_trv_sct *ncd; /* [sct] Pointer to "non-coordinate dimension" (mutually exclusive from "crd" )*/
    int id; /* [ID] Unique ID for dimension; same as "dmn_trv_sct.id", obtained from API "nc_inq_vardimid" */
  } var_dmn_sct; 
 
  /* GTT Object structure 
     Information for each object/node in traversal tree
     Contains basic information about this object/node needed by traversal algorithm
     Node/object is either group or variable like in HDF5
     Initialize trv_sct structure to defaults in trv_tbl_init()
     Populate trv_sct structure with correct values in nco_grp_itr()
     Deep-copy each pointer member of trv_sct structure in nco_grp_itr()
     free() each pointer member of trv_sct structure in trv_tbl_free() 
     */
  typedef struct{ 
    char *nm_fll; /* [sng] Fully qualified name (path) */
    var_dmn_sct var_dmn[NC_MAX_DIMS]; /* [sct] (For variables only) Dimensions for variable object */
    nco_bool is_crd_var; /* [flg] (For variables only) Is a coordinate variable? (unique dimension exists in scope) */
    nco_bool is_rec_var; /* [flg] (For variables only) Is a record variable? (is_crd_var must be True) */
    nc_type var_typ;     /* [enm] (For variables only) NetCDF type  */  
    size_t nm_fll_lng; /* [sng] Length of full name */
    char *grp_nm_fll; /* [sng] Full group name (for groups, same as nm_fll) */
    char nm[NC_MAX_NAME+1L]; /* [sng] Relative name (i.e., variable name or last component of path name for groups) */
    size_t nm_lng; /* [sng] Length of short name */
    nco_obj_typ nco_typ; /* [enm] netCDF4 object type: group or variable */
    int grp_dpt; /* [nbr] Depth of group (root = 0) */
    int grp_id_in; /* [id] Group ID in input file */
    int grp_id_out; /* [id] Group ID in output file */
    int nbr_att; /* [nbr] Number of attributes */
    int nbr_dmn; /* [nbr] Number of dimensions */
    int nbr_grp; /* [nbr] Number of sub-groups (for group) */
    int nbr_rec; /* [nbr] Number of record dimensions */
    int nbr_var; /* [nbr] Number of variables (for group) */
    nco_bool flg_cf; /* [flg] Object matches CF-metadata extraction criteria */
    nco_bool flg_crd; /* [flg] Object matches coordinate extraction criteria */
    nco_bool flg_dfl; /* [flg] Object meets default subsetting criteria */
    nco_bool flg_gcv; /* [flg] Group contains matched variable */
    nco_bool flg_mch; /* [flg] Object matches user-specified strings */
    nco_bool flg_ncs; /* [flg] Group is ancestor of specified group or variable */
    nco_bool flg_nsx; /* [flg] Object matches intersection criteria */
    nco_bool flg_rcr; /* [flg] Extract group recursively */
    nco_bool flg_unn; /* [flg] Object matches union criteria */
    nco_bool flg_vfp; /* [flg] Variable matches full path specification */
    nco_bool flg_vsg; /* [flg] Variable selected because group matches */
    nco_bool flg_xcl; /* [flg] Object matches exclusion criteria */
    nco_bool flg_xtr; /* [flg] Extract object */
   } trv_sct;
 
  /* GTT (Group Traversal Table) structure contains two lists
     1) lst: All objects (variables and groups) in file tree (HDF5 model)
     2) lst_dmn: All unique dimensions (in groups) in file tree (netCDF addition to HDF5) */
  typedef struct{
    trv_sct *lst;           /* [sct] Array of trv_sct */
    unsigned int nbr;       /* [nbr] Number of trv_sct elements */   
    dmn_trv_sct *lst_dmn;   /* [sct] Array of dmn_trv_sct */
    unsigned int nbr_dmn;   /* [nbr] Number of dmn_trv_sct elements */
  } trv_tbl_sct;

  /* Chunking structure */
  typedef struct{ /* cnk_sct */
    char *nm; /* [sng] Dimension name */
    size_t sz; /* [nbr] Chunk size */
    nco_bool is_usr_spc_cnk; /* [flg] Chunk size was user-specified */
  } cnk_sct;
  
  /* Fill actual value of dmn_sct structure in in nco_dmn_fll()
     free() each pointer member of dmn_sct structure in nco_dmn_free()
     deep-copy each pointer member of dmn_sct structure in nco_dmn_dpl() */
  /* Dimension structure */
  typedef struct dmn_sct_tag{ /* dmn_sct */
    char *nm; /* [sng] Dimension name */
    char fmt[5]; /* [sng] Hint for printf()-style formatting */
    int cid; /* [id] Variable ID of associated coordinate, if any */
    int id; /* [id] Dimension ID */
    int nc_id; /* [id] File ID */
    long cnt; /* [nbr] Number of valid elements in this dimension (including effects of stride and wrapping) */
    long end; /* [idx] Index to end of hyperslab */
    long srd; /* [nbr] Stride of hyperslab */
    long srt; /* [idx] Index to start of hyperslab */
    long sz; /* [nbr] Full size of dimension in file (NOT the hyperslabbed size) */
    nc_type type; /* [enm] Type of coordinate, if applicable */
    ptr_unn val; /* [sct] Buffer to hold hyperslab fxm: is this ever used? */
    short is_crd_dmn; /* [flg] Is this a coordinate dimension? */
    short is_rec_dmn; /* [flg] Is this the record dimension? */
    size_t cnk_sz; /* [nbr] Chunk size */
    struct dmn_sct_tag *xrf; /* [sct] Cross-reference to associated dimension structure (usually the structure for dimension on output) */
  } dmn_sct; /* end dmn_sct_tag */
  
  /* Initialize default value of each member of var_sct structure in var_dfl_set()
     Fill actual value of var_sct structure in nco_var_fll()
     free() each pointer member of var_sct structure in nco_var_free()
     deep-copy each pointer member of var_sct structure in nco_var_dpl() */
  /* Variable structure */
  typedef struct var_sct_tag{ /* var_sct */
    char *nm; /* [sng] Variable name */
    char fmt[5]; /* [sng] Hint for printf()-style formatting */
    dmn_sct **dim; /* [sct] Pointers to full dimension structures */
    int *dmn_id; /* [id] Contiguous vector of dimension IDs */
    int cid; /* [id] Dimension ID of associated coordinate, if any */
    int dfl_lvl; /* [enm] Deflate level [0..9] */
    int has_add_fst; /* [flg] Valid add_offset attribute exists */
    int has_dpl_dmn; /* [flg] Variable has duplicate copies of same dimension */
    int has_mss_val; /* [flg] Is there a missing_value attribute? */
    int has_scl_fct; /* [flg] Valid scale_factor attribute exists */
    int id; /* [id] Variable ID */
    int nbr_att; /* [nbr] Number of attributes */
    int nbr_dim; /* [nbr] Number of dimensions of variable in input file */
    int nc_id; /* [id] File ID */
    int pck_dsk; /* [flg] Variable is packed on disk (valid scale_factor, add_offset, or both attributes exist) */
    int pck_ram; /* [flg] Variable is packed in memory (valid scale_factor, add_offset, or both attributes exist) */
    int shuffle; /* [flg] Turn on shuffle filter */
    int undefined; /* [flg] Variable is still undefined (in first parser pass) */
    long *cnt; /* [nbr] Contiguous vector of lengths of hyperslab */
    long *end; /* [idx] Contiguous vector of indices to end of hyperslab */
    long *srd; /* [nbr] Contiguous vector of stride of hyperslab */
    long *srt; /* [idx] Contiguous vector of indices to start of hyperslab */
    long *tally; /* [nbr] Number of valid operations performed so far */
    long sz; /* [nbr] Number of elements (NOT bytes) in hyperslab (NOT full size of variable in input file!) */
    long sz_rec; /* [nbr] Number of elements in one record of hyperslab */
    nc_type typ_dsk; /* [enm] Type of variable on disk (never changes) */
    nc_type typ_pck; /* [enm] Type of variable when packed (on disk). typ_pck = typ_dsk except in cases where variable is packed in input file and unpacked in output file. */
    nc_type typ_upk; /* [enm] Type of variable when unpacked (expanded) (in memory) */
    nc_type type; /* [enm] Type of variable in RAM */
    ptr_unn add_fst; /* [frc] Value of add_offset attribute of type typ_upk */
    ptr_unn mss_val; /* [frc] Value of missing_value attribute, if any (mss_val stored in this structure must be same type as variable) */
    ptr_unn scl_fct; /* [frc] Value of scale_factor attribute of type typ_upk */
    ptr_unn val; /* [bfr] Buffer to hold hyperslab */
    short is_crd_var; /* [flg] Is this a coordinate variable? */
    short is_fix_var; /* [flg] Is this a fixed (non-processed) variable? */
    short is_rec_var; /* [flg] Is this a record variable? */
    size_t *cnk_sz; /* [id] Contiguous vector of chunk sizes */
    struct var_sct_tag *xrf; /* [sct] Cross-reference to associated variable structure (usually structure for variable on output) fxm: deprecate! TODO nco226 */
  } var_sct; /* end var_sct_tag */

  /* GPE duplicate name check structure */
  typedef struct{ /* gpe_nm_sct */
    char *var_nm_fll; /* Fully qualified variable name */
  } gpe_nm_sct;

  /* Limit "lmt_sct" monotonic direction */
  typedef enum {
    decreasing, /* 0 */
    increasing, /* 1 */
    not_checked /* 2 */
  } monotonic_direction_enm;
  
#ifdef __cplusplus
} /* end extern "C" */
#endif /* !__cplusplus */

/* MSVC does not define isnormal */
#ifdef _MSC_VER
# define isnormal
#endif
#endif /* NCO_H */
