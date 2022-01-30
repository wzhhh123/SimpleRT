#pragma once


#include "header.h"





class BxDF {

public:
	
	virtual dVec3 F(const dVec3& wo, const dVec3& wi) = 0;

	virtual dVec3 Sample_f(const dVec3 &wo, dVec3* wi,
		const dVec2& sample, FLOAT* pdf);


	FLOAT Pdf(const dVec3 &wo, const dVec3 &wi);
    
    
    virtual ~BxDF();

};



class BSDF{
public:
  
    void Add(BxDF* b);
    virtual dVec3 Sample_f(const dVec3 &wo, dVec3* wi,
        const dVec2& sample, FLOAT* pdf);
    
    
    virtual dVec3 F(const dVec3& wo, const dVec3& wi);
    
    virtual ~BSDF();
        
private:
    BxDF* BxDFs[8];
    int NumBxDF = 0;
};
