#include "BonusStat.h"

Tameran::BonusStat::BonusStat(int _value, int _timeStarted, float _duration, bool _timed)
	: value(_value), timeStarted(_timeStarted), duration(_duration), timed(_timed)
{}

const int Tameran::BonusStat::Value() const
{
	return value;
}

const int Tameran::BonusStat::StartingTime() const
{
	return timeStarted;
}

const float Tameran::BonusStat::Duration()
{
	return duration;
}

const bool Tameran::BonusStat::Timed() const
{
	return timed;
}

void Tameran::BonusStat::Value(int _value)
{
	value = _value;
}

void Tameran::BonusStat::StartingTime(int _timeStarted)
{
	timeStarted = _timeStarted;
}

void Tameran::BonusStat::Duration(float _duration)
{
	duration = _duration;
}

void Tameran::BonusStat::Timed(bool _timed)
{
	timed = _timed;
}
