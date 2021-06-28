#pragma once
#include "pch.h"

class SampleDrawOverrule : public AcGiDrawableOverrule
{
public:
	SampleDrawOverrule() {}
	~SampleDrawOverrule() {}
	double M_PI = 3.1415926535;
	virtual bool isApplicable(const ZcRxObject* pOverruledSubject) const override
	{
		if (pOverruledSubject->isKindOf(AcDbLine::desc())) {
			auto pLine = (AcDbLine*)pOverruledSubject;
			return (pLine->colorIndex() == 1);
		}
		return false;
	}

	virtual ZSoft::Boolean worldDraw(ZcGiDrawable* pSubject, ZcGiWorldDraw * wd) override {
		auto pline = AcDbLine::cast(pSubject);
		if (pline) {
			AcGePoint3d spt = pline->startPoint(), ept = pline->endPoint();
			drawLine(spt, ept, wd);

			double len = spt.distanceTo(ept) / 4;
			double ang = M_PI / 6;
			double angBase = (spt - ept).angleOnPlane(AcGePlane::kXYPlane);

			ads_point pt;
			acutPolar(asDblArray(ept), angBase - ang, len, pt);
			drawLine(ept, asPnt3d(pt), wd);
			acutPolar(asDblArray(ept), angBase + ang, len, pt);
			drawLine(ept, asPnt3d(pt), wd);
		}
		return Adesk::kFalse;
	}

	void drawLine(AcGePoint3d spt, AcGePoint3d ept, ZcGiWorldDraw * wd) {
		AcGePoint3d points[2] = {spt, ept};
		wd->geometry().polyline(2, points);
	}
};
