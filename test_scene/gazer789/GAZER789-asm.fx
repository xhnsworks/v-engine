//====================================================
// Shaders
// By JingWang(wjing_a@yahoo.com)
//====================================================
//--------------
// un-tweaks
//--------------
string ParamID = "0x003";
half4x4 WorldIT : WorldInverseTranspose ;
half4x4 WorldViewProj : WORLDVIEWPROJECTION ;
half4x4 World : World ;
half4x4 ViewInv : ViewInverse ;



//--------------
// Tweaks
//--------------
float3 fogColor  <
    string UIName = "FogColor ";
	string UIWidget = "Color";
>
= {1.0f, 1.0f, 1.0f};

half3 LightPos1: POSITION <
	string UIName = "Light Position";
	string Object = "PointLight";
	string Space = "World";
> = {-1.0f, 1.0f, 0.0f};



half Power1 <
    string UIWidget = "IntSpinner";
    half UIMin = 0.0;
    half UIMax = 300.0;   
    string UIName =  "Power1";
> = 1.5;

half Power2 <
    string UIWidget = "IntSpinner";
    half UIMin = 0.0;
    half UIMax = 300.0;   
    string UIName =  "Power2";
> = 1;

half Power3 <
    string UIWidget = "IntSpinner";
    half UIMin = 0.0;
    half UIMax = 300.0;  
    string UIName =  "Power3";
> = 1;


half SpecExpon  <
    string UIWidget = "IntSpinner";
    half UIMin = 1;
    half UIMax = 512.0;
    string UIName =  "Specular power";
> = 64.0;

half Bumpx<
    string UIWidget = "IntSpinner";
    half UIMin = -150.0;
    half UIMax = 150.0;   
    string UIName =  "Bumpx";
> = 0.5;

half Bumpy<
    string UIWidget = "IntSpinner";
    half UIMin = -150.0;
    half UIMax = 150.0;   
    string UIName =  "Bumpy";
> = 0.5;


half FresExp  <
    string UIWidget = "IntSpinner";
    half UIMin = 1.0;
    half UIMax = 70.0;
    string UIName =  "FresExp";
> = 3;
 
//--------------
// Textures
//--------------
texture colorSampler : Diffuse<  
	string UIName = "Diffuse Texture";
	int Texcoord = 0;
	int MapChannel = 1;			
	>;
	
texture aoSampler : Diffuse<  
	string UIName = "Ao Texture";
	int Texcoord = 0;
	int MapChannel = 1;			
	>;



texture normalSampler : Normal< 
	string UIName = "Normal Texture";
	int Texcoord = 0;
	int MapChannel = 1;	
	>;


texture env1Sampler <
    string UIName = "CubeMap1";	
    string ResourceType = "CUBE";
>;

texture env2Sampler <
    string UIName = "CubeMap2";	
    string ResourceType = "CUBE";
>;


//--------------
// vertex shader
//--------------

VertexShader VS = 
asm{
	vs_2_0
    dcl_position v0
    dcl_texcoord v1
    dcl_normal v2
    dcl_tangent v3
    dcl_binormal v4
    dp4 oT5.x, v3, c4
    dp4 oT5.y, v3, c5
    dp4 oT5.z, v3, c6
    dp4 oT6.x, v4, c4
    dp4 oT6.y, v4, c5
    dp4 oT6.z, v4, c6
    dp4 oPos.x, v0, c0
    dp4 oPos.y, v0, c1
    dp4 r1.x, v0, c7
    dp4 r1.y, v0, c8
    dp4 r1.z, v0, c9
    dp4 oPos.z, v0, c2
    add r0.xyz, -r1, c13
    dp4 oPos.w, v0, c3
    dp3 r2.x, r0, r0
    mov oT0, v1
    rsq r0.w, r2.x
    mov oT3.xyz, r1
    mov oT1, r0
    mov oT2.xyz, v2
    mov oT4.x, c10.w
    mov oT4.y, c11.w
    mov oT4.z, c12.w
};

//--------------
// pixel shader
//--------------
PixelShader PS = 
		asm {
  ps_2_x
    def c11, 0.166666672, -2, 3, 0.5
    def c12, 1.29999995, 0.200000003, 0.100000001, -0.75
    def c13, 2, -1, 0, 1
    def c14, 1.33333337, 1.5, 0, 0
    dcl_pp t0.xy
    dcl_pp t1.xyz
    dcl_pp t2.xyz
    dcl_pp t3.xyz
    dcl_pp t4.xyz
    dcl_pp t5.xyz
    dcl_pp t6.xyz
    dcl_2d s0
    dcl_2d s1
    dcl_2d s2
    dcl_cube s3
    dcl_cube s4
    texld r3, t0, s2
    dp3_pp r0.x, t6, t6
    rsq_pp r0.w, r0.x
    dp3_pp r1.x, t5, t5
    mul_pp r0.yz, r0.w, t6.wzyx
    mul_pp r0.x, r0.w, t6.x
    rsq_pp r0.w, r1.x
    mul_pp r2.yz, r0.w, t5.wzyx
    mul_pp r2.x, r0.w, t5.x
    mov_pp r1.x, c8.x
    mov_pp r1.y, c9.x
    mad_pp r3.xy, c13.x, r3, c13.y
    mul_pp r3.xy, r1, r3
    dp3_pp r1.x, t2, c0
    dp3_pp r1.z, t2, c1
    dp3_pp r1.y, t2, c2
    mul r0.w, r1.x, r1.x
    mad r0.w, r1.z, r1.z, r0.w
    mad_pp r0.w, r1.y, r1.y, r0.w
    mul_pp r2.xyz, r2, r3.x
    rsq_pp r0.w, r0.w
    mad_pp r1.xyz, r1, r0.w, r2
    mad_pp r0.xyz, r3.y, r0, r1
    mul r0.w, r0.x, r0.x
    mad r0.w, r0.z, r0.z, r0.w
    mad_pp r0.w, r0.y, r0.y, r0.w
    mov_pp r1.xyz, t4
    add_pp r8.yz, r1.wzyx, -t3.wzyx
    add_pp r8.x, r1.x, -t3.x
    rsq_pp r1.w, r0.w
    mul r0.w, r8.x, r8.x
    mad r0.w, r8.z, r8.z, r0.w
    mad_pp r0.w, r8.y, r8.y, r0.w
    mul_pp r7.xyz, r0, r1.w
    rsq_pp r7.w, r0.w
    mul_pp r1.xyz, r8, r7.w
    mul r0.w, r7.x, -r1.x
    mad r0.w, -r1.z, r7.z, r0.w
    mad r0.w, -r1.y, r7.y, r0.w
    add r0.w, r0.w, r0.w
    mul r1.w, r7.x, r1.x
    mad r1.w, r1.z, r7.z, r1.w
    mad_pp r6.w, r1.y, r7.y, r1.w
    mad_pp r0.xyz, r7, -r0.w, -r1
    add_pp r0.w, r6.w, r6.w
    mad_pp r3.xyz, r7, -r0.w, r1
    texld_pp r5, r0, s4
    texld_pp r4, r3, s3
    texld_pp r1, t0, s0
    texld_pp r0, t0, s1
    texld_pp r2, r7, s3
    nrm_pp r6.xyz, t1
    mad_pp r9.yz, r8.wzyx, r7.w, r6
    mad_pp r9.x, r8.x, r7.w, r6.x
    nrm_pp r8.xyz, r9
    abs_pp r4.w, r6.w
    mul r0.w, r7.x, r8.x
    mad r0.w, r8.y, r7.z, r0.w
    mad_pp r0.w, r8.z, r7.y, r0.w
    cmp_pp r5.w, -r0.w, c13.z, c13.w
    pow_pp r6.w, r0.w, c7.x
    mul r0.w, r7.x, r6.x
    mad r0.w, r6.y, r7.z, r0.w
    mad_pp r2.w, r6.z, r7.y, r0.w
    cmp_pp r0.w, -r2.w, c13.z, c13.w
    mul_pp r5.w, r5.w, r0.w
    mul_sat_pp r0.w, r2.w, r0.w
    mul r7.w, r3.x, r6.x
    mad r7.w, r3.z, r6.y, r7.w
    mad_sat_pp r7.w, r3.y, r6.z, r7.w
    mul_sat_pp r5.w, r6.w, r5.w
    pow_pp r6.w, r7.w, c7.x
    add r7.w, r5.w, r6.w
    add_pp r5.w, -r4.w, c13.w
    pow_pp r4.w, r5.w, c10.x
    mul_pp r3.xyz, r5, c6.x
    mad_pp r5.w, r4.w, c12.x, c12.y
    mul_pp r5.xyz, r3.w, r3
    mad_pp r4.w, r0.w, r0.x, c11.y
    mul_pp r3.xyz, r4, c6.x
    mad_pp r0.w, r0.w, r0.x, c13.x
    mul_pp r4.xyz, r4, c5.x
    mul_sat_pp r6.w, r4.w, c11.w
    mad_pp r3.xyz, r3, r3.w, r5
    mad_pp r4.w, r6.w, c11.y, c11.z
    mul_pp r3.xyz, r1.w, r3
    mul_pp r6.w, r6.w, r6.w
    mul r3.xyz, r7.w, r3
    mul_pp r4.w, r4.w, r6.w
    mul_pp r4.xyz, r5.w, r4
    mul_sat_pp r5.w, r0.w, c11.x
    mul_pp r5.xyz, r1.w, r4
    mad_pp r0.w, r5.w, c11.y, c11.z
    mad_pp r4.xyz, r4, r1, r5
    mul_pp r1.w, r5.w, r5.w
    mul_pp r4.xyz, r3.w, r4
    mad_pp r0.w, r0.w, r1.w, -r4.w
    mov_sat_pp r1.w, r2.w
    mul_pp r4.xyz, r4, r0.w
    mul r4.xyz, r1.w, r4
    mul_pp r2.xyz, r2, c4.x
    mul_pp r1.xyz, r1, r2
    mad r1.xyz, r1, r0.w, r4
    add_pp r0.w, r0.w, c12.w
    mad_pp r1.xyz, r3, r1.w, r1
    mov r2.y, c12.z
    mul r2.xyz, r2.y, c3
    mul_sat_pp r1.w, r0.w, c14.x
    mad r1.xyz, r1, r0, -r2
    mad_pp r0.w, r1.w, c11.y, c11.z
    max_pp r0.xyz, r1, c13.z
    mul_pp r1.w, r1.w, r1.w
    add_pp r0.xyz, r0, r0
    mad_pp r0.w, r0.w, -r1.w, c14.y
    mul_pp r0.xyz, r0, r0.w
    mov_pp r0.w, c13.w
    mov_pp oC0, r0
	};

			
//--------------
// techniques   
//--------------
   
   technique NormalMap
      {
 	pass p0
    <
	string Script = "Draw=geometry;";
     >  
      {		
    ZEnable = true; 
	CullMode = cw; 
	ZWriteEnable = true; 
 	VertexShader = <VS>;
	PixelShader = <PS>;
	VertexShaderConstant[0] = <WorldViewProj>;		
	VertexShaderConstant[4] = <WorldIT>;
	VertexShaderConstant[7] = <World>;
	VertexShaderConstant[10]  = <ViewInv>;
	VertexShaderConstant[13]  = <LightPos1>;
	PixelShaderConstant[0]=<WorldIT>;
	PixelShaderConstant[3]=<fogColor>;	
	PixelShaderConstant[4]=<Power1>;
	PixelShaderConstant[5]=<Power2>;
	PixelShaderConstant[6]=<Power3>;
	PixelShaderConstant[7]=<SpecExpon>;
	PixelShaderConstant[8]=<Bumpx>;
	PixelShaderConstant[9]=<Bumpy>;
	PixelShaderConstant[10]=<FresExp>;
    Texture[0] 				= <colorSampler>;
    Texture[1] 				= <aoSampler>;
    Texture[2] 				= <normalSampler>;
    Texture[3] 				= <env1Sampler>;
    Texture[4] 				= <env2Sampler>;

    

    MipFilter[0] = LINEAR;
    MinFilter[0] = LINEAR;
    MagFilter[0] = LINEAR;
    AddressU[0] = Wrap;
    AddressV[0] = Wrap;
	
    MipFilter[1] = LINEAR;
    MinFilter[1] = LINEAR;
    MagFilter[1] = LINEAR;
    AddressU[1] = Wrap;
    AddressV[1] = Wrap;
	
    MipFilter[2] = LINEAR;
    MinFilter[2] = LINEAR;
    MagFilter[2] = LINEAR;
    AddressU[2] = Wrap;
    AddressV[2] = Wrap;

    MipFilter[3] = LINEAR;
    MinFilter[3] = LINEAR;
    MagFilter[3] = LINEAR;
    AddressU[3] = clamp;
    AddressV[3] = clamp;
    AddressW[3] = clamp;

    MipFilter[4] = LINEAR;
    MinFilter[4] = LINEAR;
    MagFilter[4] = LINEAR;
    AddressU[4] = clamp;
    AddressV[4] = clamp;
    AddressW[4] = clamp;


      }
      }