/* -*- mode: c -*-
 *
 * Time-stamp: <2022-05-27 17:07:34 stefan>
 *
 * Mästarprov 5
 *
 * Fil:       mp5.c (mästarprov 5)
 * Ansvarig:  Stefan Niskanen Skoglund
 * Användare: ens21snd
 *
 * Se mp5.pdf
 *
 * https://github.com/skaraborgfakir/Umea_Universitet_C_programmering
 */

#include <stdio.h>

#define ANTALVAL 3
#define VALNYKURS 1
#define VARUPRISER 2
#define AVSLUTA 3

void visa_meny(float valutakurs);
int funktionsval();
float ny_valutakurs();
float mata_in_priser();
void visa_summa_och_konvertera( float summa, float valutakurs);
float mata_in_nytt_pris();

int main(int argc,char *argv[])
{
     int klart = 0;
     float valutakurs = 1.0;
     float summa = 1.0;

     printf( "\nYour shopping assistant\n");

     do
     {
	  visa_meny(valutakurs);

	  int val = funktionsval();

	  switch (val)
	  {
	  case VALNYKURS:
	       /* 1.3 i programflödet - modifiera använd valutakurs */
	       valutakurs = ny_valutakurs();
	       break;

	  case VARUPRISER:
	       /* 1.4 i programflödet - mata in olika saker och deras pris i lokal valuta */
	       /* som räknas om till SEK med vald valutakurs  */
	       summa = mata_in_priser();                /* fortsätter till användaren är nöjd */
	       visa_summa_och_konvertera( summa, valutakurs); /* räknas om med vald valutakurs till SEK och skrivs ut */
	       break;

	  case AVSLUTA:
	       klart = 1;
	       break;

	  default:
	       printf( "\nNot a valid choice!\n");
	  };
     }
     while (!klart);

     printf( "\nEnd of program!\n\n");

     return 0;
}

/*
 * Funktion: visa_menu
 *
 * mp5.pdf: Krav på implementationen: 1.1 ska implementeras som en separat funktion
 *
 * skriver ut tillgängliga funktionsval i programmet
 * och inställd/vald valutakurs
 *
 * Indata:    vald valutakurs
 * Utmatning: -
 */
void visa_meny(float valutakurs)
{
     printf( "\n");
     printf( "1. Set exchange rate in SEK (current rate: %.2f)\n", valutakurs);
     printf( "2. Convert prices from the foreign currency\n");
     printf( "3. End\n");
}

/*
 * mp5.pdf: Krav på implementationen: 1.2 ska implementeras som en separat funktion
 *
 * Indata:    -
 * Utmatning: vilken funktion i programmet som ska användas
 */
int funktionsval()
{
     int val = 0;

     printf( "\nGive your choice (1 - %d): ", ANTALVAL);
     scanf( "%d", &val);

     return val;
}

/*
 * mp5.pdf: Krav på implementationen: 1.3.1 ska implementeras som en separat funktion
 *
 * Indata:    -
 * Utmatning: vilken valutakurs som ska användas
 */
float ny_valutakurs()
{
     float ny_kurs;

     printf( "\nGive exchange rate: ");
     scanf("%f", &ny_kurs);

     return ny_kurs;
}


/*
 * mp5.pdf: Krav på implementationen: 1.4.1 ska implementeras som en separat funktion
 *
 * Funktion i 1.4.1.1 ska anropas av denna
 *
 * Indata:    -
 * Utmatning: summa av alla varornas pris (i lokal valuta)
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
 * mp5.pdf: Krav på implementationen: 1.4.1.1 ska implementeras som en separat funktion
 *          Krav på implementationen: ska anropas från mata_in_priser (1.4.1 i flödet)
 *
 * Indata:    -
 * Utmatning: någon saks pris (i lokal valuta)
 */
float mata_in_nytt_pris()
{
     float pris  = 0.0;

     printf( "Give price (finish with < 0): ");
     scanf( "%f", &pris);

     return pris;
}

/*
 * mp5.pdf: Krav på implementationen: 1.4.2 ska implementeras som en separat funktion
 *
 * Indata:    vald valutakurs
 *            summa i lokal valuta
 * Utmatning: -
 */
void visa_summa_och_konvertera( float summa,     /* i lokal valuta */
				float valutakurs /* omräkning till SEK */ )
{
     printf( "\nSum in foreign currency: %.2f\nSum in SEK: %.2f\n",
	     summa,
	     summa * valutakurs);
}



/*
 * Local Variables:
 * c-file-style: "k&r"
 * compile-command: "make"
 * End:
 */
