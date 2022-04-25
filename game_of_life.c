/*
 * Programmering i C
 * Spring 22
 * Mastery test 9
 *
 * Date:         Time-stamp: <2022-04-18 17:42:13 stefan>
 * File:         game_of_life.c
 * Description:  A simple implementation of Conway's Game of Life.
 * Author:       Stefan Niskanen Skoglund
 * CS username:  ens21snd
 *
 * ska ha själva maskinimplementationen med regler och policy från mp8.c
 * exv reglerna för hur en generation hanteras och hur nästa generation ska se ut
 */

/* livstidshantering av field struktur - använder malloc !
 */
field *allocate_field ( const int nrows,
			const int ncols)
{
  field *field_ptr;

  field_ptr
  return malloc( sizeof( cell) * nrows * ncols);
}

void dispose_field( field *current_field)
{
  free( current_field.cells;
	}

    int neighbours( field current_field,
		    const int row,
		    const int col)
    {
      return 0;
    }

  /*
   */
  void dump_field( const int nrows,
		   const int ncols,
		   cell atlas[nrows][ncols] )
  {
    for (int r = 0 ; r < nrows ; r++) {
      for (int c = 0 ; c < ncols ; c++) {
	putc( atlas[r][c].current, stdout);
      }
      putc( '\n', stdout);
    }
  }


  /* Description: Initialize all the cells to dead, then asks the user
   *              about which structure to load, and finally load the
   *              structure.
   * Input:       The field array and its size.
   * Output:      The field array is updated.
   *
   * modifierad för att använda sig av field-strukturen
   */

  void init_field( const int rows,
		   const int cols,
		   cell field[rows][cols])
  {
    for (int r = 0 ; r < rows ; r++) {
      for (int c = 0 ; c < cols ; c++) {
	field[r][c].current = DEAD;
      }
    }

    printf("Select field spec to load ");
    printf("([G]lider, [S]emaphore, ");
    printf("[R]andom or [C]ustom): ");

    int ch = getchar();

    /* Ignore following newline */
    if (ch != '\n') {
      getchar();
    }

    switch (ch) {
    case 'g':
    case 'G':
      load_glider(rows, cols, field);
      break;
    case 's':
    case 'S':
      load_semaphore(rows, cols, field);
      break;
    case 'r':
    case 'R':
      load_random(rows, cols, field);
      break;
    case 'c':
    case 'C':
    default:
      load_custom(rows, cols, field);
    }
  }

  /*
   * Local Variables:
   * c-file-style: "k&r"
   * compile-command: "make"
   * End:
   */
