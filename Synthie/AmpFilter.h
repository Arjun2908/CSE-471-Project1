#pragma once
#include "AudioFilter.h"

class CAmpFilter :
	public CAudioFilter
{
public:
	CAmpFilter();
	virtual ~CAmpFilter();

	//! Start the node generation
	virtual void Start() override;
	//! Cause one sample to be generated
	virtual bool Generate() override;
};

