#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

// U8g2 Contructor List (Picture Loop Page Buffer)
// The complete list is available here: https://github.com/olikraus/u8g2/wiki/u8g2setupcpp
// Please update the pin numbers according to your setup. Use U8X8_PIN_NONE if the reset pin is not connected
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

/* button setup */
uint8_t pin_A = 3;
uint8_t pin_B = 4;

/*================================================================*/
/* graphics object */
/*================================================================*/

u8g2_t *st_u8g2;

u8g2_uint_t u8g_height_minus_one;


#define ST_AREA_HEIGHT (st_u8g2->height - 8)
#define ST_AREA_WIDTH (st_u8g2->width)

uint16_t st_to_diff_cnt = 0;
uint8_t form_index = 0;
uint8_t form_index_max = 2;

/*================================================================*/
/* overall game state */
/*================================================================*/

#define STATE_PREPARE 0
#define STATE_PREPARE_PROCESS 1
#define STATE_MAIN 2
#define STATE_MAIN_PROCESS 3
#define STATE_END 4
#define STATE_END_PROCESS 5

uint8_t current_state = STATE_PREPARE;

/*================================================================*/
/* API: game draw procedure */
/*================================================================*/

void st_DrawInGame(uint8_t formIndex)
{  
  u8g2_SetDrawColor(st_u8g2, 1);
  u8g2_DrawHLine(st_u8g2, 0, u8g_height_minus_one - ST_AREA_HEIGHT+1, ST_AREA_WIDTH);
  u8g2_DrawHLine(st_u8g2, 0, u8g_height_minus_one, ST_AREA_WIDTH);
  u8g2_SetFont(st_u8g2, u8g_font_4x6r);

  
  //u8g2_DrawStr(st_u8g2, 0, u8g_height_minus_one - ST_AREA_HEIGHT, st_itoa(st_difficulty));
  //u8g2_DrawHLine(st_u8g2, 10, u8g_height_minus_one - ST_AREA_HEIGHT-3, (st_to_diff_cnt>>ST_DIFF_FP)+1);
  //u8g2_DrawVLine(st_u8g2, 10, u8g_height_minus_one - ST_AREA_HEIGHT-4, 3);
  //u8g2_DrawVLine(st_u8g2, 10+ST_DIFF_VIS_LEN, u8g_height_minus_one - ST_AREA_HEIGHT-4, 3);
}

void st_Draw(void) 
{
  uint8_t current_position = u8g_height_minus_one -  (st_u8g2->height-6)/2;
  switch(current_state) {
    case STATE_PREPARE:
    case STATE_PREPARE_PROCESS:
      drawBox();
      current_position -= 10;
      u8g2_DrawStr(st_u8g2, 0, current_position, "ArduinoForms by Kayku");
      current_position += 16;
      u8g2_DrawStr(st_u8g2, 6, current_position, "Cargando formulario");
      current_position += 15;
      u8g2_DrawHLine(st_u8g2, st_u8g2->width-st_to_diff_cnt - 20, current_position, 20);
      break;
    case STATE_MAIN:
      drawBox();
      current_position -= 10;
      switch (form_index) {
        case 0:
          u8g2_DrawStr(st_u8g2, 0, current_position, "Por ser siempre leal,");
          current_position += 10;
          u8g2_DrawStr(st_u8g2, 0, current_position, "con buenos valores.");
          current_position += 10;
          u8g2_DrawStr(st_u8g2, 0, current_position, "Por ser lo que se");
          current_position += 10;
          u8g2_DrawStr(st_u8g2, 0, current_position, "llama un buen amigo.");
          break;
        case 1:
          u8g2_DrawStr(st_u8g2, 0, current_position, "Por saber aguantarme,");
          current_position += 10;
          u8g2_DrawStr(st_u8g2, 0, current_position, "y estar disponible para");
          current_position += 10;
          u8g2_DrawStr(st_u8g2, 0, current_position, "tomar algo y contarte");
          current_position += 10;
          u8g2_DrawStr(st_u8g2, 0, current_position, "todos los detalles.");
          break;
        case 2:
          u8g2_DrawStr(st_u8g2, 0, current_position, "Para que otros,");
          current_position += 10;
          u8g2_DrawStr(st_u8g2, 0, current_position, "puedan aprender de ti,");
          current_position += 10;
          u8g2_DrawStr(st_u8g2, 0, current_position, "y hacer nuestras vidas");
          current_position += 10;
          u8g2_DrawStr(st_u8g2, 0, current_position, "un tanto mejores.");
          break;
      }
      break;
    case STATE_MAIN_PROCESS:
      drawBox();
      u8g2_DrawStr(st_u8g2, 30, current_position, "¿Continuar?");
      current_position += 12;
      u8g2_DrawStr(st_u8g2, 18, current_position, ">> Si");
      u8g2_DrawStr(st_u8g2, 70, current_position, ">> No");
      break;
    case STATE_END:
    case STATE_END_PROCESS:
      drawBox();
      current_position -= 10;
      u8g2_DrawStr(st_u8g2, 25, current_position, "¡Enhorabuena!");
      current_position += 11;
      u8g2_DrawStr(st_u8g2, 0, current_position, "¡Has sido seleccionado");
      current_position += 11;
      u8g2_DrawStr(st_u8g2, 0, current_position, "como padrino de Nico!");
      current_position += 3;
      u8g2_DrawHLine(st_u8g2, st_to_diff_cnt, current_position, 20);
      break;
  }
}

void drawBox()
{
  u8g2_SetDrawColor(st_u8g2, 1);
  u8g2_SetFont(st_u8g2, u8g_font_6x10r);
  u8g2_DrawHLine(st_u8g2, 0, u8g_height_minus_one - ST_AREA_HEIGHT+1, ST_AREA_WIDTH);
  u8g2_DrawHLine(st_u8g2, 0, u8g_height_minus_one, ST_AREA_WIDTH);
}

/*================================================================*/
/* API: setup form procedure */
/*================================================================*/

void setupGame(void) 
{
  st_to_diff_cnt = 0;
}

void st_Setup(u8g2_t *u8g) 
{
  st_u8g2 = u8g;
  u8g2_SetBitmapMode(u8g, 1);
  u8g_height_minus_one = u8g->height;
  u8g_height_minus_one--;
}
  
void checkState(void)
{
  switch(current_state)
  {
    case STATE_PREPARE:
      st_to_diff_cnt = st_u8g2->width-20;
      current_state = STATE_PREPARE_PROCESS;
      break;
    case STATE_PREPARE_PROCESS:
      st_to_diff_cnt--;
      if (st_to_diff_cnt == 0 || pushedButton(pin_A))
      {
        current_state = STATE_MAIN;
        setupGame();
      }
      if (pushedButton(pin_B))
      {
        current_state = STATE_PREPARE;
      }
      break;
    case STATE_MAIN:
      if (pushedButton(pin_A))
      {
        current_state = STATE_MAIN_PROCESS;
      }
      if (pushedButton(pin_B))
      {
        form_index = 0;
        current_state = STATE_PREPARE;
      }
      break;
    case STATE_MAIN_PROCESS:
      if (pushedButton(pin_A))
      {
        if (form_index == form_index_max)
        {
          current_state = STATE_END;
        } else {
          form_index++;
          current_state = STATE_MAIN;
        }
      }
      if (pushedButton(pin_B))
      {
        form_index = 0;
        current_state = STATE_PREPARE;
      }
      break;
    case STATE_END:
      st_to_diff_cnt = st_u8g2->width-20;
      current_state = STATE_END_PROCESS;
      break;
    case STATE_END_PROCESS:
      st_to_diff_cnt--;
      if ( st_to_diff_cnt == 0 || 
          pushedButton(pin_A) || pushedButton(pin_B))
      {
        current_state = STATE_PREPARE;
      }
      break;
  }
}

boolean pushedButton(uint8_t pin)
{
  if (digitalRead(pin) == LOW) {
    digitalWrite(pin, HIGH);
    delay(500);
    return true;
  }
  return false;
}

void setup(void)
{
  pinMode(pin_A, INPUT_PULLUP);
  pinMode(pin_B, INPUT_PULLUP);
  u8g2.begin();
}


void loop(void)
{
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.setFontDirection(0);
  u8g2.setFontRefHeightAll();

  st_Setup(u8g2.getU8g2());
  
  for(;;) {
    checkState();
    u8g2.firstPage();
    
    do {
      st_Draw();
    } while( u8g2.nextPage() );
  }
}
