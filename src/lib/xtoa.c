//
// Convert integers to ASCII strings
//
static void xtoa(unsigned long val, char *buf, unsigned radix, int negative) {
    char *p;
    char *firstdig;
    char temp;
    unsigned digval;

    p = buf;

    if (negative) {
        // Negative, so output '-' and negate
        *p++ = '-';
        val = (unsigned long) (-(long) val);
    }

    // Save pointer to first digit
    firstdig = p;

    do {
        digval = (unsigned) (val % radix);
        val /= radix;

        // Convert to ascii and store
        if (digval > 9) {
            *p++ = (char) (digval - 10 + 'a');
        } else {
            *p++ = (char) (digval + '0');
        }
    } while (val > 0);

    // We now have the digit of the number in the buffer, but in reverse
    // order.  Thus we reverse them now.

    *p-- = '\0';
    do {
        temp = *p;
        *p = *firstdig;
        *firstdig = temp;
        p--;
        firstdig++;
    } while (firstdig < p);
}

char *itoa(int val, char *buf, int radix) {
    if (radix == 10 && val < 0) {
        xtoa((unsigned long) val, buf, radix, 1);
    } else {
        xtoa((unsigned long) (unsigned int) val, buf, radix, 0);
    }
    return buf;
}

char *ltoa(long val, char *buf, int radix) {
    xtoa((unsigned long) val, buf, radix, (radix == 10 && val < 0));
    return buf;
}

char *ultoa(unsigned long val, char *buf, int radix) {
    xtoa(val, buf, radix, 0);
    return buf;
}
