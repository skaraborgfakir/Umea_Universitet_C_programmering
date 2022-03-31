/*
 * Time-stamp: <2022-03-31 23:56:20 stefan>
 *
 * Stefan Niskanen Skoglund
 * ens21snd
 *
 * https://github.com/skaraborgfakir/Umea_Universitet_C_programmering
 *
 * åäö
 */

#include <stdio.h>

#define ANTALVAL 3
#define VALNYKURS 1
#define VARUPRISER 2
#define AVSLUTA 3

/*
 * mp5.pdf: punkt 1.1 i programflödet
 *
 * skriver ut tillgängliga funktionsval i programmet
 */
void visameny(float kurs)
{
     printf( "\n");
     printf( "1. Set exchange rate in SEK (current rate: %.2f)\n", kurs);
     printf( "2. Convert prices from the foreign currency\n");
     printf( "3. End\n");
}

/*
 * mp5.pdf: punkt 1.2 i programflödet
 */
int funktionsval()
{
     int val = 0;

     printf( "\n");
     printf( "Give your choice (1 - %d): ", ANTALVAL);
     scanf( "%d", &val);

     return val;
}

/*
 * mp5.pdf: punkt 1.3.1 i programflödet
 */
float valutakurs()
{
     float ny_kurs;

     printf( "\n");
     printf( "Give exchange rate: ");
     scanf("%f", &ny_kurs);

     return ny_kurs;
}

/*
 * mp5.pdf: punkt 1.4.1.1 i programflödet
 *
 * ska anropas från summerapriser (krav på implementationen)
 */
float mata_in_nytt_pris()
{
     float pris  = 0.0;

     printf( "Give price (finish with < 0): ");
     scanf( "%f", &pris);

     return pris;
}

/*
 * mp5.pdf: punkt 1.4.1 i programflödet
 * separat funktion som
 */
float mata_in_priser()
{
     float summa = 0.0;
     float pris  = 0.0;
     int   klart = 0;

     printf( "\n");

     /*
      * avsluta efter att ett negativt tal matas in
      */
     do
     {
	  pris = mata_in_nytt_pris();

	  if (pris < 0)
	  {
	       klart = 1;
	  }
	  else
	  {
	       summa += pris;
	  }
     }
     while (!klart);

     return summa;
}

/*
 * mp5.pdf: punkt 1.4.2 i programflödet
 */
void visa_summa_och_konvertera( float summa,     /* i lokal valuta */
				float valutakurs /* omräkning till SEK */ )
{
     printf( "\n");
     printf( "Sum in foreign currency: %.2f\nSum in SEK: %.2f\n",
	     summa,
	     summa * valutakurs);
}


int main(int argc,char *argv[])
{
     int done = 0;
     float kurs = 1.0;
     float summa = 1.0;

     printf( "\nYour shopping assistant\n");

     do
     {
	  visameny(kurs);

	  int val = funktionsval();

	  switch (val)
	  {
	  case VALNYKURS:
	       /* modifiera använd kurs */
	       kurs = valutakurs();
	       break;

	  case VARUPRISER:
	       /* mata in olika saker och deras pris i lokal valuta */
	       /* räkna om med kursen till SEK */
	       summa = mata_in_priser();
	       visa_summa_och_konvertera( summa, kurs);
	       break;

	  case AVSLUTA:
	       done = 1;
	       break;

	  default:
	       printf( "\n");
	       printf( "Not a valid choice!\n");
	  };
     }
     while (!done);

     printf( "\nEnd of program!\n\n");

     return 0;
}

/*
 * Local Variables:
 * c-file-style: "k&r"
 * compile-command: "/usr/bin/gcc -Wall --std=c99 mp5.c -o mp5"
 * End:
 */
