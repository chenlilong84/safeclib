/*------------------------------------------------------------------
 * wcstok_s.c
 *
 * October 2008, Bo Berry
 * September 2017, Reini Urban
 *
 * Copyright (c) 2008-2011 by Cisco Systems, Inc
 * Copyright (c) 2017 by Reini Urban
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *------------------------------------------------------------------
 */

#ifdef FOR_DOXYGEN
#include "safe_str_lib.h"
#else
#include "safeclib_private.h"
#endif

#if defined(_WIN32) && defined(HAVE_WCSTOK_S)
#else

/**
 * @brief
 *    A sequence of calls to the wcstok_s function breaks the string
 *    pointed to by dest into a sequence of tokens, each of which is
 *    delimited by a character from the string pointed to by delim. The
 *    fourth argument points to a caller-provided char pointer into
 *    which the wcstok_s function stores information necessary for
 *    it to continue scanning the same string.
 * @details
 *    The first call in a sequence has a non-null first argument and
 *    dmax points to an object whose value is the number of elements
 *    in the character array pointed to by the first argument. The
 *    first call stores an initial value in the object pointed to by
 *    ptr and updates the value pointed to by dmax to reﬂect the
 *    number of elements that remain in relation to ptr. Subsequent
 *    calls in the sequence have a null first argument and the objects
 *    pointed to by dmax and ptr are required to have the values
 *    stored by the previous call in the sequence, which are then
 *    updated. The separator string pointed to by delim may be different
 *    from call to call.
 *
 *    The first call in the sequence searches the string pointed to
 *    by dest for the first character that is not contained in the
 *    current separator string pointed to by delim. If no such character
 *    is found, then there are no tokens in the string pointed to
 *    by dest and the wcstok_s function returns a null pointer. If
 *    such a character is found, it is the start of the first token.
 *
 *    The wcstok_s function then searches from there for the first
 *    character in dest that is contained in the current separator
 *    string. If no such character is found, the current token
 *    extends to the end of the string pointed to by dest, and
 *    subsequent searches in the same string for a token return
 *    a null pointer. If such a character is found, it is
 *    overwritten by a null character, which terminates the
 *    current token.
 *
 *    In all cases, the wcstok_s function stores sufﬁcient information
 *    in the pointer pointed to by ptr so that subsequent calls,
 *    with a null pointer for dest and the unmodiﬁed pointer value
 *    for ptr, shall start searching just past the element overwritten
 *    by a null character (if any).
 *
 *    delim uses a STRTOK_DELIM_MAX_LEN of 16.
 *
 * @remark SPECIFIED IN
 *    * C11 standard (ISO/IEC 9899:2011):
 *    K.3.7.3.1 The wcstok_s function (p: 620-621)
 *    http://en.cppreference.com/w/c/string/byte/strok
 *    * ISO/IEC TR 24731-1, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *
 * @param[in]   dest    pointer to string to tokenize
 * @param[out]  dmax    restricted maximum length of dest string
 * @param[in]   delim   pointer to delimiter string (len < 255)
 * @param[out]  ptr     returned pointer to token
 *
 * @pre  delim shall not be a null pointer.
 * @pre  ptr shall not be a null pointer.
 * @pre  dmax shall not be a null pointer.
 * @pre  *dmax shall not be 0.
 * @pre  If dest is a null pointer, then *ptr shall not be a null pointer.
 * @pre  dest must not be unterminated.
 * @pre  The value of *dmax shall not be greater than RSIZE_MAX_WSTR. The
 *       end of the token found shall occur within the first *dmax
 *       characters of dest for the first call, and shall occur within
 *       the first *dmax characters of where searching resumes on
 *       subsequent calls.
 * @pre  delim must not be longer than STRTOK_DELIM_MAX_LEN (default: 16).
 *
 * @note The mingw MINGW_HAS_SECURE_API declares it without the dmax
 *       argument and without restrict. Skip it there.
 *       <tt>wchar_t* wcstok_s(wchar_t *str, const wchar_t *delim,
 *                             wchar_t **next_token)</tt>
 *
 * @note C11 uses RSIZE_MAX, not RSIZE_MAX_WSTR.
 *
 * @return  The wcstok_s function returns a pointer to the first character
 *          of a token; or a null pointer if there is no token or there
 *          is a runtime-constraint violation.
 *          Each call modifies dest by substituting a wide NULL character for the
 *          first delimiter that occurs after the returned token.
 *          If there is a runtime-constraint violation, the strtok_s function
 *          does not indirect through the dest/delim pointers, and does not
 *          store a value in the object pointed to by ptr.
 *
 * errno is set to:
 *          ESNULLP     when dest/delim/ptr is NULL pointer
 *          ESZEROL     when *dmax = 0
 *          ESLEMAX     when *dmax > RSIZE_MAX_WSTR
 *          ESUNTERM    when unterminated string
 * C11 just returns EINVAL
 *
 * @remarks 
 * Example to demonstrate usage of wcstok_s() to tokenize a string
 * @code{.c}
 *   // Approach1: sequential wcstok_s() calls
 *   str1 = L",.:*one,two;three,;four*.*.five-six***"; // String to tokenize 
 *   len = 38;
 *   str2 = L",.;*"; // String of delimiters
 *
 *   p2tok = wcstok_s(str1, &len, str2, &p2str); 
 *   // token: one, remaining: two;three,;four*.*.five-six***, len: 30
 *
 *   p2tok = wcstok_s(NULL, &len, str2, &p2str);
 *   // token: two, remaining: three,;four*.*.five-six***, len: 26
 *
 *   p2tok = wcstok_s(NULL, &len, str2, &p2str);
 *   // token: three,  remaining: ;four*.*.five-six***, len: 20
 *
 *   p2tok = wcstok_s(NULL, &len, str2, &p2str);
 *   // token: four, remaining .*.five-six***, len: 14
 *
 *   p2tok = wcstok_s(NULL, &len, str2, &p2str);
 *   // token: five-six, remaining: **, len: 2
 *
 *    p2tok = wcstok_s(NULL, &len, str2, &p2str);
 *    // token: (null), remaining: **, len: 0
 *
 *    // Approach2: Use of while loop with same entry data as used above
 *    p2tok = str1;
 *    while (p2tok && len) 
 *    {
 *      p2tok = wcstok_s(NULL, &len, str2, &p2str);
 *      printf("  token --   remaining --  len=0 \n", p2tok, p2str, (int)len );
 *    }
 * @endcode
 */

EXPORT wchar_t *
wcstok_s(wchar_t *restrict dest, rsize_t *restrict dmax,
         const wchar_t *restrict delim, wchar_t **restrict ptr)
{
    const wchar_t *pt;
    wchar_t *ptoken;
    rsize_t dlen;
    rsize_t slen;

    if (unlikely(dmax == NULL)) {
        invoke_safe_str_constraint_handler("wcstok_s: dmax is NULL",
                   NULL, ESNULLP);
        errno = ESNULLP;
        return (NULL);
    }

    if (unlikely(*dmax == 0)) {
        invoke_safe_str_constraint_handler("wcstok_s: dmax is 0",
                   NULL, ESZEROL);
        errno = ESZEROL;
        return (NULL);
    }

    if (unlikely(*dmax > RSIZE_MAX_WSTR)) {
        invoke_safe_str_constraint_handler("wcstok_s: dmax exceeds max",
                   NULL, ESLEMAX);
        errno = ESLEMAX;
        return (NULL);
    }

    if (unlikely(delim == NULL)) {
        invoke_safe_str_constraint_handler("wcstok_s: delim is null",
                   NULL, ESNULLP);
        errno = ESNULLP;
        return (NULL);
    }

    if (unlikely(ptr == NULL)) {
        invoke_safe_str_constraint_handler("wcstok_s: ptr is null",
                   NULL, ESNULLP);
        errno = ESNULLP;
        return (NULL);
    }

    /* if the source was NULL, use the tokenizer context */
    if (dest == NULL) {
        dest = *ptr;
        if (unlikely(dest == NULL)) {
            invoke_safe_str_constraint_handler("wcstok_s: dest/*ptr is null",
                                               NULL, ESNULLP);
            errno = ESNULLP;
            return (NULL);
        }
    }

    /*
     * scan dest for a delimiter
     */
    dlen = *dmax;
    ptoken = NULL;
    errno = 0;
    while (*dest != L'\0' && !ptoken) {

        if (unlikely(dlen == 0)) {
            *ptr = NULL;
            *dmax = 0;
            *dest = L'\0';
            invoke_safe_str_constraint_handler(
                      "wcstok_s: dest is unterminated",
                       NULL, ESUNTERM);
            errno = ESUNTERM;
            return (NULL);
        }

        /*
         * must scan the entire delimiter list
         * ISO should have included a delimiter string limit!!
         */
        slen = STRTOK_DELIM_MAX_LEN;
        pt = delim;
        while (*pt != L'\0') {

            if (unlikely(slen == 0)) {
                *ptr = NULL;
                *dmax = 0;
                *dest = L'\0';
                invoke_safe_str_constraint_handler(
                          "wcstok_s: delim is unterminated",
                           NULL, ESUNTERM);
                errno = ESUNTERM;
                return (NULL);
            }
            slen--;

            if (*dest == *pt) {
                ptoken = NULL;
                break;
            } else {
                pt++;
                ptoken = dest;
            }
        }
        dest++;
        dlen--;
    }

    /*
     * if the beginning of a token was not found, then no
     * need to continue the scan.
     */
    if (ptoken == NULL) {
        *dmax = dlen;
        return (ptoken);
    }

    /*
     * Now we need to locate the end of the token
     */
    while (*dest != L'\0') {

        if (unlikely(dlen == 0)) {
            *ptr = NULL;
            *dmax = 0;
            *dest = L'\0';
            invoke_safe_str_constraint_handler(
                      "wcstok_s: dest is unterminated",
                       NULL, ESUNTERM);
            errno = ESUNTERM;
            return (NULL);
        }

        slen = STRTOK_DELIM_MAX_LEN;
        pt = delim;
        while (*pt != L'\0') {

            if (unlikely(slen == 0)) {
                *ptr = NULL;
                *dmax = 0;
                *dest = L'\0';
                invoke_safe_str_constraint_handler(
                          "wcstok_s: delim is unterminated",
                           NULL, ESUNTERM);
                errno = ESUNTERM;
                return (NULL);
            }
            slen--;

            if (*dest == *pt) {
                /*
                 * found a delimiter, set to null
                 * and return context ptr to next char
                 */
                *dest = L'\0';
                *ptr = (dest + 1);  /* return pointer for next scan */
                *dmax = dlen - 1;   /* account for the nulled delimiter */
                return (ptoken);
            } else {
                /*
                 * simply scanning through the delimiter string
                 */
                pt++;
            }
        }
        dest++;
        dlen--;
    }

    *dmax = dlen;
    return (ptoken);
}
EXPORT_SYMBOL(wcstok_s)

#endif
