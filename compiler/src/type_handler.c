#include "../inc/type_handler.h"

/***********************************************************/
/*                    PRIVATE FUNCTIONS                    */
/***********************************************************/

token_t is_keyword(const char **_lxm);
token_t is_unsigned_int(const char **_lxm);
token_t is_signed_int(const char **_lxm);
token_t is_float(const char **_lxm);
token_t is_char(const char **_lxm);
token_t is_bool(const char **_lxm);
token_t is_str(const char **_lxm);

/***********************************************************/
/*                    PUBLIC FUNCTIONS                     */
/***********************************************************/
size_t str_cmp(const char *_str1, const char *_str2)
{
    while (*_str1 || *_str2)
    {
        if (*_str1 != *_str2)
        {
            return 0;
        }

        _str1++;
        _str2++;
    }

    return 1;
}

token_t deduce_tok_type(const char **_lxm)
{
    size_t res = undefined;

    res = is_keyword(_lxm);

    return res;
}

/***********************************************************/
/*                  FUNCTION DECLARATIONS                  */
/***********************************************************/

token_t is_signed_int(const char **_lxm)
{
    if (**_lxm == 'i')
    {
        if (str_cmp(*_lxm, "i8"))
        {
            return KW_I8;
        }
        else if (str_cmp(*_lxm, "i16"))
        {
            return KW_I16;
        }
        else if (str_cmp(*_lxm, "i32"))
        {
            return KW_I32;
        }
        else if (str_cmp(*_lxm, "i64"))
        {
            return KW_I64;
        }
        else if (str_cmp(*_lxm, "i128"))
        {
            return KW_I128;
        }
    }

    return undefined;
}

token_t is_unsigned_int(const char **_lxm)
{
    if (**_lxm == 'i')
    {
        if (str_cmp(*_lxm, "u8"))
        {
            return KW_U8;
        }
        else if (str_cmp(*_lxm, "u16"))
        {
            return KW_U16;
        }
        else if (str_cmp(*_lxm, "u32"))
        {
            return KW_U32;
        }
        else if (str_cmp(*_lxm, "u64"))
        {
            return KW_U64;
        }
        else if (str_cmp(*_lxm, "u128"))
        {
            return KW_U128;
        }
        else if (str_cmp(*_lxm, "usize"))
        {
            return KW_USIZE;
        }
    }

    return undefined;
}

token_t is_float(const char **_lxm)
{
    if (**_lxm == 'f')
    {
        if (str_cmp(*_lxm, "f32"))
        {
            return KW_F32;
        }
        else if (str_cmp(*_lxm, "f64"))
        {
            return KW_F64;
        }
        else if (str_cmp(*_lxm, "f128"))
        {
            return KW_F128;
        }
    }

    return undefined;
}
token_t is_bool(const char **_lxm)
{
    if (str_cmp(*_lxm, "bool"))
    {
        return KW_BOOL;
    }

    return undefined;
}

token_t is_char(const char **_lxm)
{
    if (str_cmp(*_lxm, "char"))
    {
        return KW_CHAR;
    }

    return undefined;
}

token_t is_str(const char **_lxm)
{
    if (str_cmp(*_lxm, "str"))
    {
        return KW_STR;
    }

    return undefined;
}

token_t is_keyword(const char **_lxm)
{
    token_t res = undefined;

    token_t (*keyword_checks[])(const char **) = {is_signed_int, is_unsigned_int, is_float, is_bool, is_char, is_str};
    size_t arr_size = sizeof(keyword_checks) / sizeof(keyword_checks[0]);

    for (size_t i = 0; i < arr_size; ++i)
    {
        res = keyword_checks[i](_lxm);
        if (res != undefined)
        {
            return res;
        }
    }

    return res;
}