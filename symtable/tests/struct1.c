static struct str1 {
	int a;
	struct str2 {
		short d,e;
		char c;
	} str2;
	char c;
} s1;

struct str2 s2;

struct str3 {
	struct str4 *p4;
} s3;

struct str4 {
	struct str3 *p3;
	struct str1 str1[10];
	int i;
} s4;

struct {
	int y;
} s5;


struct str6 {
	int a;
} s6;

union u1 {
	struct str4 str4;
	struct str3 str3;
} u1;

