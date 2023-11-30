#include "../inc/type_handler.h"

token_t is_keyword(const char **_lxm);
token_t is_delimiter(const char **_lxm);
token_t is_punctuation(const char **_lxm);

token_t is_str_kw(const char **_lxm);
token_t is_int_kw(const char **_lxm);
token_t is_uint_kw(const char **_lxm);
token_t is_bool_kw(const char **_lxm);
token_t is_char_kw(const char **_lxm);
token_t is_loop_kw(const char **_lxm);
token_t is_float_kw(const char **_lxm);
token_t is_return_kw(const char **_lxm);
token_t is_ctrl_flow_kw(const char **_lxm);

token_t get_type(const char **_lxm)
{
    size_t res = undefined;

    if (strlen(*_lxm) > 1)
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

    if (*_lxm)
    {
        free(_lxm);
        *_lxm = NULL;
    }

    return res;
}

token_t is_int_kw(const char **_lxm)
{
    if (**_lxm == 'i')
    {
        if (strcmp(*_lxm, "i8"))
        {
            return KW_I8;
        }
        else if (strcmp(*_lxm, "i16"))
        {
            return KW_I16;
        }
        else if (strcmp(*_lxm, "i32"))
        {
            return KW_I32;
        }
        else if (strcmp(*_lxm, "i64"))
        {
            return KW_I64;
        }
        else if (strcmp(*_lxm, "i128"))
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
        if (strcmp(*_lxm, "u8"))
        {
            return KW_U8;
        }
        else if (strcmp(*_lxm, "u16"))
        {
            return KW_U16;
        }
        else if (strcmp(*_lxm, "u32"))
        {
            return KW_U32;
        }
        else if (strcmp(*_lxm, "u64"))
        {
            return KW_U64;
        }
        else if (strcmp(*_lxm, "u128"))
        {
            return KW_U128;
        }
        else if (strcmp(*_lxm, "usize"))
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
        if (strcmp(*_lxm, "f32"))
        {
            return KW_F32;
        }
        else if (strcmp(*_lxm, "f64"))
        {
            return KW_F64;
        }
        else if (strcmp(*_lxm, "f128"))
        {
            return KW_F128;
        }
    }

    return undefined;
}
token_t is_bool_kw(const char **_lxm)
{
    if (strcmp(*_lxm, "bool"))
    {
        return KW_BOOL;
    }
    else if (strcmp(*_lxm, "true"))
    {
        return KW_TRUE;
    }
    else if (strcmp(*_lxm, "false"))
    {
        return KW_FALSE;
    }

    return undefined;
}

token_t is_char_kw(const char **_lxm)
{
    if (strcmp(*_lxm, "char"))
    {
        return KW_CHAR;
    }

    return undefined;
}

token_t is_str_kw(const char **_lxm)
{
    if (strcmp(*_lxm, "str"))
    {
        return KW_STR;
    }

    return undefined;
}

token_t is_return_kw(const char **_lxm)
{
    if (strcmp(*_lxm, "return"))
    {
        return KW_RETURN;
    }

    return undefined;
}

token_t is_loop_kw(const char **_lxm)
{
    if (strcmp(*_lxm, "while"))
    {
        return KW_WHILE;
    }
    else if (strcmp(*_lxm, "for"))
    {
        return KW_FOR;
    }
    else if (strcmp(*_lxm, "break"))
    {
        return KW_BREAK;
    }
    else if (strcmp(*_lxm, "skip"))
    {
        return KW_SKIP;
    }

    return undefined;
}

token_t is_ctrl_flow_kw(const char **_lxm)
{
    if (strcmp(*_lxm, "if"))
    {
        return KW_IF;
    }
    else if (strcmp(*_lxm, "else"))
    {
        return KW_ELSE;
    }
    else if (strcmp(*_lxm, "match"))
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
            is_return_kw,
            is_loop_kw,
            is_ctrl_flow_kw
        };

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
    if (strcmp(*_lxm, "+"))
    {
        return PUNC_PLUS;
    }
    else if (strcmp(*_lxm, "-"))
    {
        return PUNC_MINUS;
    }
    else if (strcmp(*_lxm, "/"))
    {
        return PUNC_FW_SLASH;
    }
    else if (strcmp(*_lxm, "*"))
    {
        return PUNC_STAR;
    }
    else if (strcmp(*_lxm, "%"))
    {
        return PUNC_PERCENT;
    }
    else if (strcmp(*_lxm, "^"))
    {
        return PUNC_CARET;
    }
    else if (strcmp(*_lxm, "!"))
    {
        return PUNC_NOT;
    }
    else if (strcmp(*_lxm, "&"))
    {
        return PUNC_AND;
    }
    else if (strcmp(*_lxm, "|"))
    {
        return PUNC_OR;
    }
    else if (strcmp(*_lxm, "="))
    {
        return PUNC_EQ;
    }
    else if (strcmp(*_lxm, ">"))
    {
        return PUNC_GT;
    }
    else if (strcmp(*_lxm, "<"))
    {
        return PUNC_LT;
    }
    else if (strcmp(*_lxm, "@"))
    {
        return PUNC_AT;
    }
    else if (strcmp(*_lxm, "#"))
    {
        return PUNC_HTAG;
    }
    else if (strcmp(*_lxm, ","))
    {
        return PUNC_COMMA;
    }
    else if (strcmp(*_lxm, "."))
    {
        return PUNC_DOT;
    }
    else if (strcmp(*_lxm, ";"))
    {
        return PUNC_SEMI;
    }
    else if (strcmp(*_lxm, ":"))
    {
        return PUNC_COL;
    }

    return undefined;
}

token_t is_delimiter(const char **_lxm)
{
    if (strcmp(*_lxm, "("))
    {
        return DELIM_BRACE_L;
    }
    else if (strcmp(*_lxm, ")"))
    {
        return DELIM_BRACE_R;
    }
    else if (strcmp(*_lxm, "["))
    {
        return DELIM_SQ_BRACE_L;
    }
    else if (strcmp(*_lxm, "]"))
    {
        return DELIM_SQ_BRACE_R;
    }
    else if (strcmp(*_lxm, "{"))
    {
        return DELIM_SQ_BRACE_L;
    }
    else if (strcmp(*_lxm, "}"))
    {
        return DELIM_SQ_BRACE_R;
    }

    return undefined;
}