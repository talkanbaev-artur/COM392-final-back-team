#ifndef COMMUNITY_API_H
#define COMMUNITY_API_H

class Community
{
private:
	//Social distancing factor illustrates the chance of getting infected in the community
	//temp - no texture map yet
	double sdf;

	//index of goverment intervention - lockdowns, hygiene propaganda, mask mandate, etc.
	//0 - no intervention, 1.0 - total control
	//sorry for generalisation
	double igi;

public:
	Community();
	~Community();
};

#endif