#ifndef ERGB_HPP_SENTRY
#define ERGB_HPP_SENTRY

/* Extended (signed) RGB. */

/* Format: sRsGsB, where:
 * s(1 bit)
 * R(8 bit)
 * G(8 bit)
 * B(8 bit). */

typedef unsigned int ERgb;

inline ERgb eRgb(int r, int g, int b)
{
    return
        ((r < 0) ? 0x1 : 0x0) << 26 |
        (((r < 0) ? -r : r) & 0xFF) << 18 |
        ((g < 0) ? 0x1 : 0x0) << 17 |
        (((g < 0) ? -g : g) & 0xFF) << 9 |
        ((b < 0) ? 0x1 : 0x0) << 8 |
        (((b < 0) ? -b : b) & 0xFF);
}

inline int eRed(ERgb c)
{
    return (c >> 26) & 0x1
        ? -((c >> 18) & 0xFF)
        : (c >> 18) & 0xFF;
}

inline int eGreen(ERgb c)
{
    return (c >> 17) & 0x1
        ? -((c >> 9) & 0xFF)
        : (c >> 9) & 0xFF;
}

inline int eBlue(ERgb c)
{
    return (c >> 8) & 0x1
        ? -(c & 0xFF)
        : c & 0xFF;
}

#endif /* ERGB_HPP_SENTRY */
