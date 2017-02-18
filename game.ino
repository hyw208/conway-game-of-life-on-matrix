// led matrix (8 * 8) pin to arduino pin mappings 
const int pin1 = 17;
const int pin2 = 15;
const int pin3 = 7;
const int pin4 = 8;
const int pin5 = 14;
const int pin6 = 10;
const int pin7 = 16;
const int pin8 = 4;
const int pin9 = 2;
const int pin10 = 9;
const int pin11 = 11;
const int pin12 = 5;
const int pin13 = 6;
const int pin14 = 3;
const int pin15 = 12;
const int pin16 = 13;

// led matrix pin to row/col mappings
const int row1 = pin9;
const int row2 = pin14;
const int row3 = pin8;
const int row4 = pin12;
const int row5 = pin1;
const int row6 = pin7;
const int row7 = pin2;
const int row8 = pin5;
const int col1 = pin13;
const int col2 = pin3;
const int col3 = pin4;
const int col4 = pin10;
const int col5 = pin6;
const int col6 = pin11;
const int col7 = pin15;
const int col8 = pin16;

// easy access by index later
const int rows[8] = {row1, row2, row3, row4, row5, row6, row7, row8};
const int cols[8] = {col1, col2, col3, col4, col5, col6, col7, col8};

// matrix alive or empty states
int matrix[8][8] = {};

// hold next gen matrix alive or empty states
int next_matrix[8][8] = {};

// light up leds based on matrix states
void render() 
{
  for (int row=0; row<8; row++) 
  {
    digitalWrite(rows[row], HIGH);
    for (int col=0; col<8; col++) 
    {
      int led = matrix[row][col];
      // for cell(i, j) to light up, keep row i HIGH and column j LOW
      digitalWrite(cols[col], led == 1 ? LOW : HIGH);
      if (led == 1) 
      {
        digitalWrite(cols[col], HIGH);
      }
    }
    digitalWrite(rows[row], LOW);
  }
}

// dim the entire matrix
void dim() 
{
  for (int row=0; row<8; row++) 
  {
    digitalWrite(rows[row], LOW);
    for (int col=0; col<8; col++) 
    {
      digitalWrite(cols[col], HIGH);
    }  
  }
}

// randomize the matrix inital states for conway game of live
void chance()
{
  for (int row=0; row<8; row++) 
  {
    for (int col=0; col<8; col++) 
    {
      matrix[row][col] = random(2);
    } 
  }
}

// seed the matrix inital states 
void some_pattern()
{
  for (int row=0; row<8; row++) 
  {
    for (int col=0; col<8; col++) 
    {
      matrix[row][col] = 0;
    } 
  }
  matrix[2][2] = 1;
  matrix[1][2] = 1;
  matrix[0][2] = 1;
  matrix[2][1] = 1;
  matrix[1][0] = 1;
}

// calculate new matrix states
void next_gen() 
{
  for (int row=0; row<8; row++)
  {
    for (int col=0; col<8; col++)
    {
      // check neighbor count
      int count = 0;
      int lrow = row - 1 >= 0 ? row - 1 : row, hrow = row + 1 < 8 ? row + 1 : row;
      int lcol = col - 1 >= 0 ? col - 1 : col, hcol = col + 1 < 8 ? col + 1 : col;
      for (int r=lrow; r<=hrow; r++)
      {
        for (int c=lcol; c<=hcol; c++)
        {
          if (r != row || c != col) 
          {
            if (matrix[r][c] == 1)
            {
              count += 1;
            } 
          }  
        }
      }
      
      // based on neighbor count, decide to live or die
      int cell = matrix[row][col];      
      if (cell == 1) // live cell
      {
        if (count < 2 || count > 3)
        {
          cell = 0; // to die
        }
      }
      else // empty cell
      {
        if (count > 2)
        {
          cell = 1; // to live 
        }
      }
      
      // update new matrix state
      next_matrix[row][col] = cell;
    }  
  }
   
  // ** update matrix in one go
  for (int row=0; row<8; row++)
  {
    for (int col=0; col<8; col++)
    {
      matrix[row][col] = next_matrix[row][col];
    }
  }
}

// arduino life cycle method: setup is called once
void setup() 
{
  // set arduino pin mode
  for (int i=2; i<18; i++)
  {
    pinMode(i, OUTPUT);
  }
  
  // setup initial pattern for matrix
  some_pattern();
  
  // dim entire matrix
  dim();

  // debug to console if needed
  Serial.begin(9600);
  Serial.println("Game on!");
}

int loops = 0; // think of one loop is one day
int lifespan = 365 * 3; // 365 * 3 days per generation

// arduino life cycle method: it's called very rapidly, treat one call as one day
void loop() 
{
  // just another day
  loops += 1; 

  // next generation is to come
  if (loops % lifespan == 0) 
  {
    loops = 0;
    next_gen();
  } 
  
  // called every loop to keep showing the matrix states
  render();
}




