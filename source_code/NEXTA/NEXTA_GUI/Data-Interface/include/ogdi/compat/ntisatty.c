static int ttyset = 0;
static int ttyvalue;

int
compat_isatty(int x)
{
    if (ttyset) {
	return ttyvalue;
    } else {
	return isatty(x);
    }
}

void
compat_setisatty(int value)
{
    ttyset = 1;
    ttyvalue = value;
}
