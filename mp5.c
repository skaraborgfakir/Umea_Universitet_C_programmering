/*
 * Time-stamp: <2022-03-31 11:34:51 stefan>
 */

#include <stdio.h>

#define ANTALVAL 3
#define VALNYKURS 1
#define VARUPRISER 2
#define AVSLUTA 3

/*
 * mp5.pdf: punkt 1.1 i programflödet
 */
void visameny()
{
     char *huvud = "Your shopping assistant";
     struct val {
	  int valnr;
	  char *beskrivning;
     };
     struct val tillgängligaval[ANTALVAL] = {
	  { VALNYKURS,  "Set exchange rate" },
	  { VARUPRISER, "Convert prices from the foreign currency" },
	  { AVSLUTA,    "End" }
     };

     printf( "%s\n\n", huvud);
     for (int i = 0 ; i < ANTALVAL ; i++)
     {
	  printf( "%d. %s\n",
		  tillgängligaval[i].valnr,
		  tillgängligaval[i].beskrivning);
     }
}

/*
 * mp5.pdf: punkt 1.2 i programflödet
 */
int läsval()
{
     return 0;
}

/*
 * mp5.pdf: punkt 1.3.1 i programflödet
 */
float valutakurs()
{
     return 10.0;
}

/*
 * mp5.pdf: punkt 1.4.1 i programflödet
 */
float summerapriser()
{
     return 10.0;
}

/*
 * mp5.pdf: punkt 1.4.1.1 i programflödet
 */
void frågaefterpris()
{
}

/*
 * mp5.pdf: punkt 1.4.2 i programflödet
 */
void summerasummaochkonvertera( float summa,     /* i lokal valuta */
				float valutakurs /* omräkning till SEK */ )
{
     printf( "summa: %f (lokal valuta) %f (SEK)\n",
	     summa,
	     summa * valutakurs);
}


int main(int argc,char *argv[])
{
     int stop = 0;

     do
     {
	  visameny();

	  stop = 1;
     }
     while (!stop);

     return 0;
}

/*
 * Local Variables:
 * c-file-style: "k&r"
 * compile-command: "/usr/bin/gcc mp5.c -o mp5"
 * End:
 */
