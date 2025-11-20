struct who_entry
{
    string user<32>;
    string tty<32>;
    string host<256>;
    long login_time;
};

struct who_list
{
    who_entry entries<64>;
};

program WHO_PROG
{
    version WHO_VERS
    {
        who_list WHO_PROC(void) = 1;
    }
    = 1;
}
= 0x31234567;
