#include "../inc/token_handler.h"

/***********************************************************/
/*                    PRIVATE FUNCTIONS                    */
/***********************************************************/

token_t is_keyword(const char **_lxm);
token_t is_punctuation(const char **_lxm);
token_t is_delimiter(const char **_lxm);

token_t is_int_kw(const char **_lxm);
token_t is_uint_kw(const char **_lxm);
token_t is_float_kw(const char **_lxm);
token_t is_bool_kw(const char **_lxm);
token_t is_char_kw(const char **_lxm);
token_t is_str_kw(const char **_lxm);
token_t is_permission_kw(const char **_lxm);
token_t is_return_kw(const char **_lxm);
token_t is_loop_kw(const char **_lxm);
token_t is_ctrl_flow_kw(const char **_lxm);

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

// https://elixir.bootlin.com/glibc/glibc-2.17/source/string/strlen.c
size_t str_len(const char *_str)
{
    const char *char_ptr;
    const unsigned long int *longword_ptr;
    unsigned long int longword, himagic, lomagic;

    for (char_ptr = _str; ((unsigned long int)char_ptr & (sizeof(longword) - 1)) != 0;
         ++char_ptr)
        if (*char_ptr == '\0')
            return char_ptr - _str;

    longword_ptr = (unsigned long int *)char_ptr;
    himagic = 0x80808080L;
    lomagic = 0x01010101L;

    if (sizeof(longword) > 4)
    {
        himagic = ((himagic << 16) << 16) | himagic;
        lomagic = ((lomagic << 16) << 16) | lomagic;
    }

    if (sizeof(longword) > 8)
    {
        abort();
    }

    for (;;)
    {
        longword = *longword_ptr++;

        if (((longword - lomagic) & ~longword & himagic) != 0)
        {
            const char *cp = (const char *)(longword_ptr - 1);

            if (cp[0] == 0)
            {
                return cp - _str;
            }
            if (cp[1] == 0)
            {
                return cp - _str + 1;
            }
            if (cp[2] == 0)
            {
                return cp - _str + 2;
            }
            if (cp[3] == 0)
            {
                return cp - _str + 3;
            }
            if (sizeof(longword) > 4)
            {
                if (cp[4] == 0)
                {
                    return cp - _str + 4;
                }
                if (cp[5] == 0)
                {
                    return cp - _str + 5;
                }
                if (cp[6] == 0)
                {
                    return cp - _str + 6;
                }
                if (cp[7] == 0)
                {
                    return cp - _str + 7;
                }
            }
        }
    }
}

token_t deduce_tok_type(const char **_lxm)
{
    size_t res = undefined;

    if (str_len(*_lxm) > 1)
    {
        res = is_keyword(_lxm);
    }
    else if (res == undefined)
    {
        res = is_punctuation(_lxm);
    }
    else
    {
        res = is_delimiter(_lxm);
    }

    return res;
}

/***********************************************************/
/*                  FUNCTION DECLARATIONS                  */
/***********************************************************/

token_t is_int_kw(const char **_lxm)
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

token_t is_uint_kw(const char **_lxm)
{
    if (**_lxm == 'u')
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

token_t is_float_kw(const char **_lxm)
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
token_t is_bool_kw(const char **_lxm)
{
    if (str_cmp(*_lxm, "bool"))
    {
        return KW_BOOL;
    }
    else if (str_cmp(*_lxm, "true"))
    {
        return KW_TRUE;
    }
    else if (str_cmp(*_lxm, "false"))
    {
        return KW_FALSE;
    }

    return undefined;
}

token_t is_char_kw(const char **_lxm)
{
    if (str_cmp(*_lxm, "char"))
    {
        return KW_CHAR;
    }

    return undefined;
}

token_t is_str_kw(const char **_lxm)
{
    if (str_cmp(*_lxm, "str"))
    {
        return KW_STR;
    }

    return undefined;
}

token_t is_permission_kw(const char **_lxm)
{
    if (**_lxm == '/')
    {
        if (str_cmp(*_lxm, "/r"))
        {
            return KW_READ_ONLY;
        }
        else if (str_cmp(*_lxm, "/rx"))
        {
            return KW_READ_ONLY_UNIQUE;
        }
        else if (str_cmp(*_lxm, "/rs"))
        {
            return KW_READ_ONLY_SHARED;
        }
        else if (str_cmp(*_lxm, "/rw"))
        {
            return KW_READ_AND_WIRTE;
        }
        else if (str_cmp(*_lxm, "/rwx"))
        {
            return KW_READ_AND_WIRTE_UNIQUE;
        }
        else if (str_cmp(*_lxm, "/rws"))
        {
            return KW_READ_AND_WIRTE_SHARED;
        }
    }

    return undefined;
}

token_t is_return_kw(const char **_lxm)
{
    if (str_cmp(*_lxm, "return"))
    {
        return KW_RETURN;
    }

    return undefined;
}

token_t is_loop_kw(const char **_lxm)
{
    if (str_cmp(*_lxm, "while"))
    {
        return KW_WHILE;
    }
    else if (str_cmp(*_lxm, "for"))
    {
        return KW_FOR;
    }
    else if (str_cmp(*_lxm, "break"))
    {
        return KW_BREAK;
    }
    else if (str_cmp(*_lxm, "skip"))
    {
        return KW_SKIP;
    }

    return undefined;
}

token_t is_ctrl_flow_kw(const char **_lxm)
{
    if (str_cmp(*_lxm, "if"))
    {
        return KW_IF;
    }
    else if (str_cmp(*_lxm, "else"))
    {
        return KW_ELSE;
    }
    else if (str_cmp(*_lxm, "match"))
    {
        return KW_MATCH;
    }

    return undefined;
}

token_t is_keyword(const char **_lxm)
{
    token_t res = undefined;

    token_t (*keyword_checks[])(const char **) =
        {
            is_int_kw,
            is_uint_kw,
            is_float_kw,
            is_bool_kw,
            is_char_kw,
            is_str_kw,
            is_permission_kw,
            is_return_kw,
            is_loop_kw,
            is_ctrl_flow_kw};

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

token_t is_punctuation(const char **_lxm)
{
    if (str_cmp(*_lxm, "+"))
    {
        return PUNC_PLUS;
    }
    else if (str_cmp(*_lxm, "-"))
    {
        return PUNC_MINUS;
    }
    else if (str_cmp(*_lxm, "/"))
    {
        return PUNC_FW_SLASH;
    }
    else if (str_cmp(*_lxm, "*"))
    {
        return PUNC_STAR;
    }
    else if (str_cmp(*_lxm, "%"))
    {
        return PUNC_PERCENT;
    }
    else if (str_cmp(*_lxm, "^"))
    {
        return PUNC_CARET;
    }
    else if (str_cmp(*_lxm, "!"))
    {
        return PUNC_NOT;
    }
    else if (str_cmp(*_lxm, "&"))
    {
        return PUNC_AND;
    }
    else if (str_cmp(*_lxm, "|"))
    {
        return PUNC_OR;
    }
    else if (str_cmp(*_lxm, "="))
    {
        return PUNC_EQ;
    }
    else if (str_cmp(*_lxm, ">"))
    {
        return PUNC_GT;
    }
    else if (str_cmp(*_lxm, "<"))
    {
        return PUNC_LT;
    }
    else if (str_cmp(*_lxm, "@"))
    {
        return PUNC_AT;
    }
    else if (str_cmp(*_lxm, "#"))
    {
        return PUNC_HTAG;
    }
    else if (str_cmp(*_lxm, ","))
    {
        return PUNC_COMMA;
    }
    else if (str_cmp(*_lxm, "."))
    {
        return PUNC_DOT;
    }
    else if (str_cmp(*_lxm, ";"))
    {
        return PUNC_SEMI;
    }
    else if (str_cmp(*_lxm, ":"))
    {
        return PUNC_COL;
    }

    return undefined;
}

token_t is_delimiter(const char **_lxm)
{
    if (str_cmp(*_lxm, "("))
    {
        return DELIM_BRACE_L;
    }
    else if (str_cmp(*_lxm, ")"))
    {
        return DELIM_BRACE_R;
    }
    else if (str_cmp(*_lxm, "["))
    {
        return DELIM_SQ_BRACE_L;
    }
    else if (str_cmp(*_lxm, "]"))
    {
        return DELIM_SQ_BRACE_R;
    }
    else if (str_cmp(*_lxm, "{"))
    {
        return DELIM_SQ_BRACE_L;
    }
    else if (str_cmp(*_lxm, "}"))
    {
        return DELIM_SQ_BRACE_R;
    }

    return undefined;
}