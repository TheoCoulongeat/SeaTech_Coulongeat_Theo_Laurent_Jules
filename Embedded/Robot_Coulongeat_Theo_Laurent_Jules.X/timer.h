#ifndef TIMER_H
#define TIMER_H
void InitTimer23(void);
void InitTimer1(void);
void SetFreqTimer1(float);
void InitTimer4(void);
void SetFreqTimer4(float);
extern volatile unsigned long timestamp;
#endif /* TIMER_H */