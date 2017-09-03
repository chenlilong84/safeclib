/*------------------------------------------------------------------
 * test_vsnwprintf_s
 *
 *------------------------------------------------------------------
 */

#include "test_private.h"
#include "safe_str_lib.h"
#include <stdarg.h>

#define LEN   ( 128 )

static wchar_t   str1[LEN];
static wchar_t   str2[LEN];

int vtwprintf_s (wchar_t *restrict dest, rsize_t dmax,
                const wchar_t *restrict fmt, ...) {
    int rc;
    va_list ap;
    va_start(ap, fmt);
    rc = vsnwprintf_s(dest, dmax, fmt, ap);
    va_end(ap);
    return rc;
}
 
int test_vsnwprintf_s (void)
{
    errno_t rc;
    int32_t  ind;
    size_t  len2;
    size_t  len3;
    int errs = 0;

/*--------------------------------------------------*/

    /* not testable
      rc = vtwprintf_s(str1, LEN, "%s", NULL);
      ERR(ESNULLP)
    */

/*--------------------------------------------------*/

    rc = vtwprintf_s(str1, LEN, NULL, NULL);
    ERR(ESNULLP);

/*--------------------------------------------------*/

    rc = vtwprintf_s(str1, 0, L"%ls", str2);
    ERR(ESZEROL)

/*--------------------------------------------------*/

    rc = vtwprintf_s(str1, (RSIZE_MAX_STR+1), L"%ls", str2);
    ERR(ESLEMAX)

/*--------------------------------------------------*/

    wcscpy(str1, L"aaaaaaaaaa");
    wcscpy(str2, L"keep it simple");

    len2 = wcslen(str2);

    rc = vtwprintf_s(str1, 50, L"%ls", str2);
    ERR((errno_t)len2)
    len3 = wcslen(str1);
    if (len3 != len2) {
#ifdef DEBUG
        size_t len1 = wcslen(str1);
#endif
        debug_printf("%s %u lengths wrong: %lu  %lu  %lu \n",
                     __FUNCTION__, __LINE__, len1, len2, len3);
        errs++;
    }

/*--------------------------------------------------*/

    str1[0] = '\0';
    wcscpy(str2, L"keep it simple");

    rc = vtwprintf_s(str1, 1, L"%ls", str2);
    NOERR() /* truncation */
    WEXPNULL(str1)

/*--------------------------------------------------*/

    str1[0] = '\0';
    wcscpy(str2, L"keep it simple");

    rc = vtwprintf_s(str1, 2, L"%ls", str2);
    NOERR() /* truncation */
    WEXPSTR(str1, L"k");

/*--------------------------------------------------*/

    str1[0] = '\0';
    wcscpy(str2, L"keep it simple");

    rc = vtwprintf_s(str1, 20, L"%ls", str2);
    NOERR()
    WEXPSTR(str1, str2)

/*--------------------------------------------------*/

    str1[0] = '\0';
    str2[0] = '\0';

    rc = vtwprintf_s(str1, LEN, L"%ls", str2);
    ERR(0)
    WEXPNULL(str1)

/*--------------------------------------------------*/

    str1[0] = '\0';
    wcscpy(str2, L"keep it simple");

    rc = vtwprintf_s(str1, LEN, L"%ls", str2);
    NOERR()
    WEXPSTR(str1, str2)

/*--------------------------------------------------*/

    wcscpy(str1, L"qqweqq");
    wcscpy(str2, L"keep it simple");

    rc = vtwprintf_s(str1, LEN, L"%ls", str2);
    NOERR()
    WEXPSTR(str1, str2)

/*--------------------------------------------------*/

    wcscpy(str1, L"1234");
    wcscpy(str2, L"keep it simple");

    rc = vtwprintf_s(str1, 12, L"%ls", str2);
    NOERR() /* truncation */
    WEXPSTR(str1, L"keep it sim")

/*--------------------------------------------------*/

    wcscpy(str1, L"1234");
    wcscpy(str2, L"keep it simple");

    rc = vtwprintf_s(str1, 52, L"%ls", str2);
    NOERR()
    WEXPSTR(str1, str2)

/*--------------------------------------------------*/

    wcscpy(str1, L"12345678901234567890");

    rc = vtwprintf_s(str1, 8, L"%ls", &str1[7]);
    NOERR() /* truncation, overlapping allowed */
    WEXPSTR(str1, L"8901234")

/*--------------------------------------------------*/

    wcscpy(str1, L"123456789");

    rc = vtwprintf_s(str1, 9, L"%ls", &str1[8]);
    ERR(1) /* overlapping allowed */
    WEXPSTR(str1, L"9")

/*--------------------------------------------------*/

    wcscpy(str2, L"123");
    wcscpy(str1, L"keep it simple");

    rc = vtwprintf_s(str2, 31, L"%ls", &str1[0]);
    NOERR()
    WEXPSTR(str2, L"keep it simple");

/*--------------------------------------------------*/

    wcscpy(str2, L"1234");
    wcscpy(str1, L"56789");

    rc = vtwprintf_s(str2, 10, L"%ls", str1);
    NOERR()
    WEXPSTR(str2, L"56789")

/*--------------------------------------------------*/

    return (errs);
}

#ifndef __KERNEL__
/* simple hack to get this to work for both userspace and Linux kernel,
   until a better solution can be created. */
int main (void)
{
    return (test_vsnwprintf_s());
}
#endif
