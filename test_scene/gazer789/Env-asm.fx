//====================================================
// Shaders
// By JingWang(wjing_a@yahoo.com)
//====================================================
//--------------
// un-tweaks
//--------------
string ParamID = "0x003";

half4x4 WorldViewProj : WORLDVIEWPROJECTION ;
half4x4 World : World ;
half4x4 ViewInv : ViewInverse ;



//--------------
// Tweaks
//--------------
float4 AmbiColor : Ambient
<
    string UIName =  "Ambient Light Color";
> = {1.0f, 1.0f, 1.0f, 1.0f};

//--------------
// Textures
//--------------
texture cubeMap<
    string UIName = "CubeMap";	
    string ResourceType = "CUBE";
>;

//--------------
// vertex shader
//--------------

VertexShader VS = 
asm{
	 vs_2_0
    def c10, 1, 0, 0, 0
    dcl_position v0
    mad r0, v0.xyzx, c10.xxxy, c10.yyyx
    dp4 oPos.x, r0, c0
    dp4 oPos.y, r0, c1
    dp4 oPos.z, r0, c2
    mov r1.x, c7.w
    mov r1.y, c8.w
    mov r1.z, c9.w
    dp4 r2.x, r0, c4
    dp4 r2.y, r0, c5
    dp4 r2.z, r0, c6
    dp4 oPos.w, r0, c3
    add oT0.xyz, r1, -r2
};

//--------------
// pixel shader
//--------------
PixelShader PS = 
		asm {
 ps_2_0
    def c1, 1, -1, 1, 0
    dcl t0.xyz
    dcl_cube s0
    mul r0.yz, t0.wzyx, c1
    mul r0.x, t0.x, c1.x
    texld r0, r0, s0
    mul r0, r0, c0
    mov oC0, r0
	};

			
//--------------
// techniques   
//--------------
   
   technique passthru
      {
 	pass p0
    <
	string Script = "Draw=geometry;";
     >  
      {		
    ZEnable = true;
	ZWriteEnable = true;
	CullMode = None;
	AlphaBlendEnable = false;
 	VertexShader = <VS>;
	PixelShader = <PS>;
	VertexShaderConstant[0] = <WorldViewProj>;		
	VertexShaderConstant[4] = <World>;
	VertexShaderConstant[7]  = <ViewInv>;	
	PixelShaderConstant[0]=<AmbiColor>;	
    Texture[0] 				= <cubeMap>;

    MipFilter[0] = LINEAR;
    MinFilter[0] = LINEAR;
    MagFilter[0] = LINEAR;
    AddressU[0] = clamp;
    AddressV[0] = clamp;
    AddressW[0] = clamp;

  


      }
      }