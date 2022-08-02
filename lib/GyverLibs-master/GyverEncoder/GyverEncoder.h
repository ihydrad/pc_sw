#ifndef GyverRC_h
#define GyverRC_h
#include <Arduino.h>

/*
Текущая версия: 2.0 от 01.08.2018
GyverEncoder - библиотека для отработки энкодера. Возможности:
Отработка поворота с антидребезгом
Отработка нажатия кнопки с антидребезгом
Отработка нажатия и удержания кнопки
Отработка "нажатого поворота"
Пример использования в папке examples, показывает все возможности библиотеки
*/

#define debounce_turn 30
#define debounce_button 100
#define hold_timer 800

class Encoder
{
  public:
    Encoder(uint8_t, uint8_t, uint8_t);
	
	void invert();	
	void tick();
	void setType(boolean type);
	
	boolean isTurn();	
	boolean isRight();
	boolean isLeft();	
	boolean isRightH();
	boolean isLeftH();
	
	boolean isPress();
	boolean isRelease();
    boolean isHolded();
	boolean isHold();
	
  private:
    byte _CLK, _DT, _SW;
	
    boolean DT_now, DT_last, SW_state, hold_flag, butt_flag, turn_flag;
	boolean isRight_f, isLeft_f, isRightH_f, isLeftH_f, isTurn_f;
	boolean isPress_f, isRelease_f, isHolded_f, isHold_f;
	boolean _type, _new_step = true;
	unsigned long debounce_timer;
};
 
#endif