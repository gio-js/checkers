/**
 *	Generate a new checkers table with random values
 
t_Table generate_checkers_table() {

	t_Table table;
	bool is_even_row = false, is_even_column = false;

	for (int x  =0 ; x < CELL_NUMBERS; x++) {
		is_even_row = (x % 2) == 0;

		for (int y = 0; y < CELL_NUMBERS; y++) {
			is_even_column = (y % 2) == 0;

			t_Cell cell;
			
			if (is_even_row)
				cell.CellType = (is_even_column ? CELL_TYPE.WHITE : CELL_TYPE.BLACK);
			else
				cell.CellType = (is_even_column ? CELL_TYPE.BLACK : CELL_TYPE.WHITE);
				
			cell.X = x;
			cell.Y = y;
			cell.Pawn = po
			            table.Cells[x][y] =
		}

	}

}*/
