#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

enum OutputType
  {
    analog,
    digital
  };

enum IOState
  {
    low  = false,
    high = true
  };


enum ActionType
  {
    timer = 0,
    drop,
    output,
    trigger,
    flash,
    MAX_ACTION_TYPE
  };

bool debug = true;

int analogX = 0;
int analogY = 0;
int px      = 0;
int py      = 0;
int yInc    = 0;
int xInc    = 0;
int prevPx  = 0;
int prevPy  = 0;

// //////////////////////////////////////////////////////////////////////////

class Action
{
 public:
  Action()    {}
  ~Action()   {}

 private:
  OutputType outputType;
  int outputNbr;
  IOState idleState;
 public:
  int getOutputType()                           { return this->outputType;        }
  void setOutputType(OutputType _outputType)    { this->outputType = _outputType; }

  int getOutputNbr()                            { return this->outputNbr;         }
  void setOutputNbr(int _outputNbr)             { this->outputNbr = _outputNbr;   }

  IOState getIdleState()                        { return this->idleState;         }
  void setIdleState(IOState _idleState)         { this->idleState = _idleState;   }

 private:
  int preDelay;
  void setPreDelay(int predelay)                { this->preDelay = predelay;      }
 public:
  int getPreDelay()                             { return this->preDelay;          }

 private:
  int postDelay;
  void setPostDelay(int postdelay)              { this->postDelay = postdelay;    }
 public:
  int getPostDelay()                            { return this->postDelay;         }

 public:
  void trigger()
  {
    // set out put to 0
    delay(this->preDelay);
    // set out put to 1
    delay(this->postDelay);
  }

};

//
//////////////////////////////////////////////////////////////////////////
class TimeLine { public: TimeLine() { memset(cycles, 0, 100 *
					       sizeof(Action*)); } ~TimeLine() {}

 public:
  void setCurrentCursorPosition(int pos)
  {

  }

  void addAction()
  {
    Action *c = new Action();
    this->actions[this->actionNb] = c;
    this->actionNb++;
  }

  void removeLastAction()
  {
    this->actionNB--;
    if (this->currentPage == this->actionNB)
      {
	this->currentPage--;
	px -= this->posPerPage;
      }
    delete this->cycles[this->actionNB];
    this->cycles[this->actionNB] = NULL;
  }

  void setYincrement(int ync)
  {

  }

  void displayAction()
  {

  }

  void buttonAction(bool buttonState)
  {

  }

  void startTimeLine()
  {

  }

 private:
  int currentAction;
  int ActionCursorPosition;

 private:
  int actionNb = 0;
  Action* actions[100];

};

// //////////////////////////////////////////////////////////////////////////
bool btn = true;

void buttonPressCallback()
{
  btn = true;
  //  Serial.println("pressed");
}

void buttonReleaseCallback()
{
  btn = false;
  //  Serial.println("released");
}
// //////////////////////////////////////////////////////////////////////////

TimeLine *timeLine = NULL;

void setup()
{
  //Serial.begin(9600);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  // Utiliser le scanner I2C pour trouver le port série sur lequel se trouve votre écran
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)

  // joystick input
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);

  attachInterrupt(digitalPinToInterrupt(2), buttonReleaseCallback, RISING);
  attachInterrupt(digitalPinToInterrupt(3), buttonPressCallback, FALLING);

  timeLine = new TimeLine();

}

void loop()
{
  // do input stuff
  analogX = analogRead(A6);
  analogY = analogRead(A7);

  // handle raw input
  if (analogX > 1000)
    px++;
  if (analogX < 25)
    px--;

  if (analogY > 1000)
    py++;
  if (analogY < 25)
    py--;

  xInc = px - prevPx;
  yInc = py - prevPy;

  prevPx = px;
  prevPy = py;

  // clear display
  display.clearDisplay();

  // do display stuff

  timeLine->setCurrentPosX(px);

  timeLine->setYincrement(yInc);

  timeLine->buttonAction(btn);

  timeLine->displayPage();


  // debug
  if (debug)
    {
      if (btn)
	{
	  display.setCursor(10, 10);
	  display.setTextSize(1);
	  display.setTextColor(WHITE);
	  display.print("toto");
	}


      display.setCursor(115, 25);
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.print(px);

      display.setCursor(115 - 20, 25);
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.print(xInc);

      display.setCursor(115 - 40, 25);
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.print(py);

      display.setCursor(115 - 60, 25);
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.print(yInc);
    }
  display.display();

  delay(80);
}

void testdrawcircle(void) {
  for (int16_t i = 0; i < display.height(); i += 2) {
    display.drawCircle(display.width() / 2, display.height() / 2, i, WHITE);
    display.display();
  }
}

void testfillrect(void) {
  uint8_t color = 1;
  for (int16_t i = 0; i < display.height() / 2; i += 3) {
    // alternate colors
    display.fillRect(i, i, display.width() - i * 2, display.height() - i * 2, color % 2);
    display.display();
    color++;
  }
}

void testdrawtriangle(void) {
  for (int16_t i = 0; i < min(display.width(), display.height()) / 2; i += 5) {
    display.drawTriangle(display.width() / 2, display.height() / 2 - i,
			 display.width() / 2 - i, display.height() / 2 + i,
			 display.width() / 2 + i, display.height() / 2 + i, WHITE);
    display.display();
  }
}

void testfilltriangle(void) {
  uint8_t color = WHITE;
  for (int16_t i = min(display.width(), display.height()) / 2; i > 0; i -= 5) {
    display.fillTriangle(display.width() / 2, display.height() / 2 - i,
			 display.width() / 2 - i, display.height() / 2 + i,
			 display.width() / 2 + i, display.height() / 2 + i, WHITE);
    if (color == WHITE) color = BLACK;
    else color = WHITE;
    display.display();
  }
}

void testdrawrect(void) {
  for (int16_t i = 0; i < display.height() / 2; i += 2) {
    display.drawRect(i, i, display.width() - 2 * i, display.height() - 2 * i, WHITE);
    display.display();
  }
}

void testscrolltext(void) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  display.clearDisplay();
  display.println("scroll");
  display.display();

  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
}
