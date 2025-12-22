	.file	"main.c"
	.text
	.section	.rodata
	.align 4
	.type	SDTX_DEFAULT_CONTEXT, @object
	.size	SDTX_DEFAULT_CONTEXT, 4
SDTX_DEFAULT_CONTEXT:
	.long	65537
	.align 32
	.type	vs_source_glsl410, @object
	.size	vs_source_glsl410, 1590
vs_source_glsl410:
	.ascii	"#version 410\n\nuniform vec4 vs_params[12];\nlayout(location"
	.ascii	" = 0) in vec3 position;\nlayout(location = 0) out vec4 color"
	.ascii	";\nlayout(location = 1) in vec4 color0;\nlayout(location = 1"
	.ascii	") out vec3 normal;\nlayout(location = 2) in vec3 normal0;\nl"
	.ascii	"ayout(location = 2) out vec3 fragPos;\nlayout(location = 3) "
	.ascii	"out vec3 viewSpacePos;\nlayout(location = 4) out vec3 viewSp"
	.ascii	"aceNorm;\nlayout(location = 5) out vec2 texcoord;\nlayout(lo"
	.ascii	"cation = 3) in vec2 texcoord0;\n\nfloat QISQRT(float _x)\n{\n"
	.ascii	"    float _32 = uintBitsToFloat(1597463007u - (floatBitsToUi"
	.ascii	"nt(_x) >> uint(1)));\n    return _32 * (1.5 - (((0.5 * _x) *"
	.ascii	" _32) * _32));\n}\n\nvec3 WNORM(vec3 _x)\n{\n    float param"
	.ascii	" = ((_x.x * _x.x) + (_x.y * _x.y)) + (_x.z * _x.z);\n    flo"
	.ascii	"at param_1 = ((_x.x * _x.x) + (_x.y * _x.y)) + (_x.z * _x.z)"
	.ascii	";\n    float param_2 = ((_x.x * _x.x) + (_x.y * _x.y)) + (_x"
	.ascii	".z * _x.z);\n    return vec3(_x.x * QISQRT(param), _x.y * QI"
	.ascii	"SQRT(param_1), _x.z * QISQRT(param_2));\n}\n\nvoid main()\n{"
	.ascii	"\n    vec4 _138 = vec4(position, 1.0);\n    gl_Position = ma"
	.ascii	"t4(vs_params[0], vs_params[1], vs_params[2], vs_params[3]) *"
	.ascii	" _138;\n    color = color0;\n    mat4 _150 = mat4(vs_params["
	.ascii	"4], vs_params[5], vs_params[6], vs_params[7]);\n    vec3 par"
	.ascii	"am = transpose(inverse(mat3(_150[0].xyz, _150[1].xyz, _150[2"
	.ascii	"].xyz))) * normal0;\n    normal = WNORM(param);\n    fragPos"
	.ascii	" = vec3((_150 * _138).xyz);\n    mat4 _184 = mat4"
	.string	"(vs_params[8], vs_params[9], vs_params[10], vs_params[11]);\n    viewSpacePos = vec3((_184 * vec4(fragPos, 1.0)).xyz);\n    vec3 param_1 = mat3(_184[0].xyz, _184[1].xyz, _184[2].xyz) * normal;\n    viewSpaceNorm = WNORM(param_1);\n    texcoord = texcoord0;\n}\n\n"
	.align 32
	.type	fs_source_glsl410, @object
	.size	fs_source_glsl410, 3076
fs_source_glsl410:
	.ascii	"#version 410\n\nstruct fs_params\n{\n    vec3 camPos;\n    i"
	.ascii	"nt numLights;\n};\n\nuniform fs_params _179;\n\nuniform vec4"
	.ascii	" light_params[128];\nuniform sampler2D _texture_smp;\n\nlayo"
	.ascii	"ut(location = 1) out vec4 depth_color;\nlayout(location = 2)"
	.ascii	" out vec4 normal_color;\nlayout(location = 4) in vec3 viewSp"
	.ascii	"aceNorm;\nlayout(location = 3) out vec4 position_color;\nlay"
	.ascii	"out(location = 3) in vec3 viewSpacePos;\nlayout(location = 1"
	.ascii	") in vec3 normal;\nlayout(location = 2) in vec3 fragPos;\nla"
	.ascii	"yout(location = 5) in vec2 texcoord;\nlayout(location = 0) o"
	.ascii	"ut vec4 frag_color;\nlayout(location = 0) in vec4 color;\nfl"
	.ascii	"oat near;\nfloat far;\n\nfloat QISQRT(float _x)\n{\n    floa"
	.ascii	"t _46 = uintBitsToFloat(1597463007u - (floatBitsToUint(_x) >"
	.ascii	"> uint(1)));\n    return _46 * (1.5 - (((0.5 * _x) * _46) * "
	.ascii	"_46));\n}\n\nvec3 WNORM(vec3 _x)\n{\n    float param = ((_x."
	.ascii	"x * _x.x) + (_x.y * _x.y)) + (_x.z * _x.z);\n    float param"
	.ascii	"_1 = ((_x.x * _x.x) + (_x.y * _x.y)) + (_x.z * _x.z);\n    f"
	.ascii	"loat param_2 = ((_x.x * _x.x) + (_x.y * _x.y)) + (_x.z * _x."
	.ascii	"z);\n    return vec3(_x.x * QISQRT(param), _x.y * QISQRT(par"
	.ascii	"am_1), _x.z * QISQRT(param_2));\n}\n\nfloat QSQRT(float _x)\n"
	.ascii	"{\n    float param = _x;\n    return _x * QISQRT(param);\n}\n"
	.ascii	"\nvoid depth()\n{\n    depth_color = vec4(gl_FragCoord.z, gl"
	.ascii	"_FragCoord.z, gl_FragCoord.z, 1.0);\n}\n\nvoid normal_c()\n{"
	.ascii	"\n    normal_color = vec4((viewSpaceNorm * 0.5) + vec3(0.5),"
	.ascii	" 1.0);\n}\n\nvoid position()\n{\n    position_color = vec4(v"
	.ascii	"iewSpacePos, 1.0);\n}\n\nvoid main()\n{\n    near = 0.100000"
	.ascii	"001490116119384765625;\n    far = 100.0;\n    vec3 param = n"
	.ascii	"ormal;\n    vec3 _175 = WNORM(param);\n    vec3 param_1 = _1"
	.ascii	"79.camPos - fragPos;\n    vec3 _188 = WNORM(param_1);\n    v"
	.ascii	"ec3 ad = vec3(0.0);\n    vec3 as = vec3(0.0);\n    vec4 _223"
	.ascii	" = texture(_texture_smp, texcoord);\n    int _230 = clamp(_1"
	.ascii	"79.numLights, 0, 64);\n    for (int i = 0; i < _230; i++)\n "
	.ascii	"   {\n        float range = light_params[i * 1 + 0].w;\n    "
	.ascii	"    if (light_params[i * 1 + 0].w <= 0.001000000047497451305"
	.ascii	"389404296875)\n        {\n            range = 10.0;\n       "
	.ascii	" }\n        vec3 lightColor = light_params[i * 1 + 64].xyz;\n"
	.ascii	"        float _277 = light_params[i * 1 + 64].w * 1.5;\n    "
	.ascii	"    float intensity = _277;\n        if (_277 <= 0.0)\n     "
	.ascii	"   {\n            intensity = 1.0;\n        }\n        light"
	.ascii	"Color *= intensity;\n        vec3 _288 = vec3(light_params[i"
	.ascii	" * 1 + 0].xyz) - fragPos;\n        float param_2 = dot(_288,"
	.ascii	" _288);\n        float _294 = QSQRT(param_2);\n        float"
	.ascii	" _315 = (1.0 / max(_294 * _294, 0.00999999977648258209228515"
	.ascii	"625)) * pow(clamp(1.0 - (_294 / range), 0.0, 1.0), 2.0);\n  "
	.ascii	"      vec3 param_3 = _288;\n        vec3 _321 = WNORM(param_"
	.ascii	"3);\n        vec3 param_4 = _321 + _188;\n        ad += (((l"
	.ascii	"ightColor * vec3(0.699999988079071044921875)) * max(dot(_175"
	.ascii	", _321), 0.0)) * _315);\n        as += ((((lightColor * vec3"
	.ascii	"(0.5)) * pow(max(dot(_175, WNORM(param_4)), 0.0), 32.0)) * _"
	.ascii	"315) * 0.80000001192092895507812"
	.string	"5);\n    }\n    vec3 _372 = (vec3(0.00200000009499490261077880859375) + (ad * _223.xyz)) + as;\n    frag_color = vec4(pow(_372 / (vec3(1.0) + _372), vec3(0.449999988079071044921875)) * vec3(color.xyz), _223.w);\n    depth();\n    normal_c();\n    position();\n}\n\n"
	.align 32
	.type	OELOW_vs_source_glsl410, @object
	.size	OELOW_vs_source_glsl410, 1668
OELOW_vs_source_glsl410:
	.ascii	"#version 410\n\nuniform vec4 OELOW_vs_params[12];\nlayout(lo"
	.ascii	"cation = 0) in vec3 position;\nlayout(location = 0) out vec4"
	.ascii	" color;\nlayout(location = 1) in vec4 color0;\nlayout(locati"
	.ascii	"on = 1) out vec3 normal;\nlayout(location = 2) in vec3 norma"
	.ascii	"l0;\nlayout(location = 2) out vec3 fragPos;\nlayout(location"
	.ascii	" = 3) out vec3 viewSpacePos;\nlayout(location = 4) out vec3 "
	.ascii	"viewSpaceNorm;\nlayout(location = 5) out vec2 texcoord;\nlay"
	.ascii	"out(location = 3) in vec2 texcoord0;\n\nfloat QISQRT(float _"
	.ascii	"x)\n{\n    float _32 = uintBitsToFloat(1597463007u - (floatB"
	.ascii	"itsToUint(_x) >> uint(1)));\n    return _32 * (1.5 - (((0.5 "
	.ascii	"* _x) * _32) * _32));\n}\n\nvec3 WNORM(vec3 _x)\n{\n    floa"
	.ascii	"t param = ((_x.x * _x.x) + (_x.y * _x.y)) + (_x.z * _x.z);\n"
	.ascii	"    float param_1 = ((_x.x * _x.x) + (_x.y * _x.y)) + (_x.z "
	.ascii	"* _x.z);\n    float param_2 = ((_x.x * _x.x) + (_x.y * _x.y)"
	.ascii	") + (_x.z * _x.z);\n    return vec3(_x.x * QISQRT(param), _x"
	.ascii	".y * QISQRT(param_1), _x.z * QISQRT(param_2));\n}\n\nvoid ma"
	.ascii	"in()\n{\n    vec4 _138 = vec4(position, 1.0);\n    gl_Positi"
	.ascii	"on = mat4(OELOW_vs_params[0], OELOW_vs_params[1], OELOW_vs_p"
	.ascii	"arams[2], OELOW_vs_params[3]) * _138;\n    color = color0;\n"
	.ascii	"    mat4 _150 = mat4(OELOW_vs_params[4], OELOW_vs_params[5],"
	.ascii	" OELOW_vs_params[6], OELOW_vs_params[7]);\n    vec3 param = "
	.ascii	"transpose(inverse(mat3(_150[0].xyz, _150[1].xyz, _150[2].xyz"
	.ascii	"))) * normal0;\n    normal = WNORM(param);\n    fragPos = ve"
	.ascii	"c3((_150 * _138).xyz);\n    mat4 _184 = mat4(OELOW_vs_params"
	.ascii	"[8], OEL"
	.string	"OW_vs_params[9], OELOW_vs_params[10], OELOW_vs_params[11]);\n    viewSpacePos = vec3((_184 * vec4(fragPos, 1.0)).xyz);\n    vec3 param_1 = mat3(_184[0].xyz, _184[1].xyz, _184[2].xyz) * normal;\n    viewSpaceNorm = WNORM(param_1);\n    texcoord = texcoord0;\n}\n\n"
	.align 32
	.type	OELOW_fs_source_glsl410, @object
	.size	OELOW_fs_source_glsl410, 2289
OELOW_fs_source_glsl410:
	.ascii	"#version 410\n\nstruct OELOW_fs_params\n{\n    vec3 camPos;\n"
	.ascii	"    int numLights;\n};\n\nuniform OELOW_fs_params _172;\n\nu"
	.ascii	"niform vec4 OELOW_light_params[128];\nuniform sampler2D _tex"
	.ascii	"ture_smp;\n\nlayout(location = 1) out vec4 depth_color;\nlay"
	.ascii	"out(location = 2) out vec4 normal_color;\nlayout(location = "
	.ascii	"4) in vec3 viewSpaceNorm;\nlayout(location = 3) out vec4 pos"
	.ascii	"ition_color;\nlayout(location = 3) in vec3 viewSpacePos;\nla"
	.ascii	"yout(location = 1) in vec3 normal;\nlayout(location = 2) in "
	.ascii	"vec3 fragPos;\nlayout(location = 5) in vec2 texcoord;\nlayou"
	.ascii	"t(location = 0) out vec4 frag_color;\nlayout(location = 0) i"
	.ascii	"n vec4 color;\n\nfloat QISQRT(float _x)\n{\n    float _41 = "
	.ascii	"uintBitsToFloat(1597463007u - (floatBitsToUint(_x) >> uint(1"
	.ascii	")));\n    return _41 * (1.5 - (((0.5 * _x) * _41) * _41));\n"
	.ascii	"}\n\nvec3 WNORM(vec3 _x)\n{\n    float param = ((_x.x * _x.x"
	.ascii	") + (_x.y * _x.y)) + (_x.z * _x.z);\n    float param_1 = ((_"
	.ascii	"x.x * _x.x) + (_x.y * _x.y)) + (_x.z * _x.z);\n    float par"
	.ascii	"am_2 = ((_x.x * _x.x) + (_x.y * _x.y)) + (_x.z * _x.z);\n   "
	.ascii	" return vec3(_x.x * QISQRT(param), _x.y * QISQRT(param_1), _"
	.ascii	"x.z * QISQRT(param_2));\n}\n\nfloat QSQRT(float _x)\n{\n    "
	.ascii	"float param = _x;\n    return _x * QISQRT(param);\n}\n\nvoid"
	.ascii	" depth()\n{\n    depth_color = vec4(gl_FragCoord.z, gl_FragC"
	.ascii	"oord.z, gl_FragCoord.z, 1.0);\n}\n\nvoid normal_c()\n{\n    "
	.ascii	"normal_color = vec4((viewSpaceNorm * 0.5) + vec3(0.5), 1.0);"
	.ascii	"\n}\n\nvoid position()\n{\n    position_color = vec4(viewSpa"
	.ascii	"cePos, 1.0);\n}\n\nvoid main()\n{\n    vec3 param = _172.cam"
	.ascii	"Pos - fragPos;\n    vec3 ad = vec3(0.0);\n    vec4 _201 = te"
	.ascii	"xture(_texture_smp, texcoord);\n    int _208 = clamp(_172.nu"
	.ascii	"mLights, 0, 64);\n    for (int i = 0; i < _208; i++)\n    {\n"
	.ascii	"        vec3 _235 = vec3(OELOW_light_params[i * 1 + 0].xyz) "
	.ascii	"- fragPos;\n        float param_1 = dot(_235, _235);\n      "
	.ascii	"  float _247 = QSQRT(param_1);\n        vec3 param_2 = vec3("
	.ascii	"OELOW_light_params[i * 1 + 0].xyz) - fragPos;\n        ad +="
	.ascii	" (((vec3((OELOW_light_params[i * 1 + 64] * (OELOW_light_para"
	.ascii	"ms[i * 1 + 64].w * 1.5)).xyz) * vec3(0.699999988079071044921"
	.ascii	"875)) * max(dot(normal, WNORM(param_2)), 0.0)) * (1.0 / (_24"
	.ascii	"7"
	.string	" * _247)));\n    }\n    vec3 _303 = ad * _201.xyz;\n    frag_color = vec4(pow((vec3(0.00200000009499490261077880859375) + _303) / (_303 + vec3(1.00199997425079345703125)), vec3(0.5)) * vec3(color.xyz), _201.w);\n    depth();\n    normal_c();\n    position();\n}\n\n"
	.align 32
	.type	OEquad_vs_source_glsl410, @object
	.size	OEquad_vs_source_glsl410, 250
OEquad_vs_source_glsl410:
	.string	"#version 410\n\nlayout(location = 0) out vec2 texcoord0;\nlayout(location = 1) in vec2 OEquad_texcoord;\nlayout(location = 0) in vec2 OEquad_position;\n\nvoid main()\n{\n    texcoord0 = OEquad_texcoord;\n    gl_Position = vec4(OEquad_position, 0.0, 1.0);\n}\n\n"
	.align 32
	.type	OEquad_fs_source_glsl410, @object
	.size	OEquad_fs_source_glsl410, 240
OEquad_fs_source_glsl410:
	.string	"#version 410\n\nuniform sampler2D OEquad_texture_OEquad_smp;\n\nlayout(location = 0) in vec2 texcoord0;\nlayout(location = 0) out vec4 frag_color;\n\nvoid main()\n{\n    frag_color = vec4(texture(OEquad_texture_OEquad_smp, texcoord0).xyz, 1.0);\n}\n\n"
	.align 32
	.type	OEFXAA_vs_source_glsl410, @object
	.size	OEFXAA_vs_source_glsl410, 252
OEFXAA_vs_source_glsl410:
	.string	"#version 410\n\nlayout(location = 0) out vec2 uv;\nlayout(location = 1) in vec2 OEFXAAQuad_texcoord;\nlayout(location = 0) in vec2 OEFXAAQuad_position;\n\nvoid main()\n{\n    uv = OEFXAAQuad_texcoord;\n    gl_Position = vec4(OEFXAAQuad_position, 0.0, 1.0);\n}\n\n"
	.align 32
	.type	OEFXAA_fs_source_glsl410, @object
	.size	OEFXAA_fs_source_glsl410, 2197
OEFXAA_fs_source_glsl410:
	.ascii	"#version 410\n\nuniform vec4 OEFXAA_resolution[1];\nuniform "
	.ascii	"sampler2D OEFXAA_texture_OEFXAA_sampler;\n\nlayout(location "
	.ascii	"= 0) in vec2 uv;\nlayout(location = 0) out vec4 frag_color;\n"
	.ascii	"\nvoid main()\n{\n    vec2 _26 = vec2(1.0 / OEFXAA_resolutio"
	.ascii	"n[0].x, 1.0 / OEFXAA_resolution[0].y);\n    float _98 = dot("
	.ascii	"texture(OEFXAA_texture_OEFXAA_sampler, uv).xyz, vec3(0.29899"
	.ascii	"99949932098388671875, 0.58700001239776611328125, 0.114000000"
	.ascii	"059604644775390625));\n    float _101 = dot(texture(OEFXAA_t"
	.ascii	"exture_OEFXAA_sampler, uv + (_26 * vec2(-1.0))).xyz, vec3(0."
	.ascii	"2989999949932098388671875, 0.58700001239776611328125, 0.1140"
	.ascii	"00000059604644775390625));\n    float _104 = dot(texture(OEF"
	.ascii	"XAA_texture_OEFXAA_sampler, uv + (_26 * vec2(1.0, -1.0))).xy"
	.ascii	"z, vec3(0.2989999949932098388671875, 0.587000012397766113281"
	.ascii	"25, 0.114000000059604644775390625));\n    float _107 = dot(t"
	.ascii	"exture(OEFXAA_texture_OEFXAA_sampler, uv + (_26 * vec2(-1.0,"
	.ascii	" 1.0))).xyz, vec3(0.2989999949932098388671875, 0.58700001239"
	.ascii	"776611328125, 0.114000000059604644775390625));\n    float _1"
	.ascii	"10 = dot(texture(OEFXAA_texture_OEFXAA_sampler, uv + _26).xy"
	.ascii	"z, vec3(0.2989999949932098388671875, 0.587000012397766113281"
	.ascii	"25, 0.114000000059604644775390625));\n    float _134 = _101 "
	.ascii	"+ _104;\n    float _139 = -(_134 - (_107 + _110));\n    floa"
	.ascii	"t _146 = (_101 + _107) - (_104 + _110);\n    vec2 _180 = cla"
	.ascii	"mp(vec2(_139, _146) * (1.0 / (min(abs(_139), abs(_146)) + ma"
	.ascii	"x(((_134 + _107) + _110) * 0.03125, 0.0078125))), vec2(-8.0)"
	.ascii	", vec2(8.0)) * _26;\n    vec3 _204 = (texture(OEFXAA_texture"
	.ascii	"_OEFXAA_sampler, uv + (_180 * (-0.16666667163372039794921875"
	.ascii	"))).xyz + texture(OEFXAA_texture_OEFXAA_sampler, uv + (_180 "
	.ascii	"* 0.16666667163372039794921875)).xyz) * 0.5;\n    vec3 _230 "
	.ascii	"= ((texture(OEFXAA_texture_OEFXAA_sampler, uv + (_180 * (-0."
	.ascii	"5))).xyz + texture(OEFXAA_texture_OEFXAA_sampler, uv + (_180"
	.ascii	" * 0.5)).xyz) * 0.25) + (_204 * 0.5);\n    float _233 = dot("
	.ascii	"_230, vec3(0.2989999949932098388671875, 0.587000012397766113"
	.ascii	"28125, 0.114000000059604644775390625));\n "
	.string	"   bvec3 _247 = bvec3((_233 < min(_98, min(min(_101, _104), min(_107, _110)))) || (_233 > max(_98, max(max(_101, _104), max(_107, _110)))));\n    frag_color = vec4(vec3(_247.x ? _204.x : _230.x, _247.y ? _204.y : _230.y, _247.z ? _204.z : _230.z), 1.0);\n}\n\n"
	.align 32
	.type	OEBQuad_vs_source_glsl410, @object
	.size	OEBQuad_vs_source_glsl410, 240
OEBQuad_vs_source_glsl410:
	.string	"#version 410\n\nlayout(location = 0) out vec2 uv;\nlayout(location = 1) in vec2 OEBQuad_texcoord;\nlayout(location = 0) in vec2 OEBQuad_position;\n\nvoid main()\n{\n    uv = OEBQuad_texcoord;\n    gl_Position = vec4(OEBQuad_position, 0.0, 1.0);\n}\n\n"
	.align 32
	.type	OEBQuad_fs_source_glsl410, @object
	.size	OEBQuad_fs_source_glsl410, 1043
OEBQuad_fs_source_glsl410:
	.ascii	"#version 410\n\nuniform vec4 OEBloom_params[1];\nuniform sam"
	.ascii	"pler2D OEBTex_OEBSmp;\n\nlayout(location = 0) in vec2 uv;\nl"
	.ascii	"ayout(location = 0) out vec4 frag_color;\n\nvoid main()\n{\n"
	.ascii	"    vec2 _26 = vec2(1.0 / OEBloom_params[0].z, 1.0 / OEBloom"
	.ascii	"_params[0].w);\n    vec3 _48 = texture(OEBTex_OEBSmp, uv).xy"
	.ascii	"z;\n    vec2 _120 = uv + (_26 * vec2(-1.0, 1.0));\n    frag_"
	.ascii	"color = vec4(_48 + (((((((((((_48 + texture(OEBTex_OEBSmp, u"
	.ascii	"v + (_26 * vec2(-1.0))).xyz) + texture(OEBTex_OEBSmp, uv + ("
	.ascii	"_26 * vec2(0.0, -1.0))).xyz) + texture(OEBTex_OEBSmp, uv + ("
	.ascii	"_26 * vec2(1.0, -1.0))).xyz) + texture(OEBTex_OEBSmp, _120)."
	.ascii	"xyz) + texture(OEBTex_OEBSmp, uv + (_26 * vec2(1.0, 0.0))).x"
	.ascii	"yz) + texture(OEBTex_OEBSmp, _120).xyz) + texture(OEBTex_OEB"
	.ascii	"Smp, uv + (_26 * vec2(0.0, 1.0))).xyz) + texture(OEBTex_OEBS"
	.ascii	"mp, uv + _26).xyz) "
	.string	"* vec3(0.111111111938953399658203125)) * clamp((dot(_48, vec3(0.2989999949932098388671875, 0.58700001239776611328125, 0.114000000059604644775390625)) - OEBloom_params[0].x) * (1.0 / (1.0 - OEBloom_params[0].x)), 0.0, 1.0)) * OEBloom_params[0].y), 1.0);\n}\n\n"
	.align 32
	.type	vs_OESSAO_source_glsl410, @object
	.size	vs_OESSAO_source_glsl410, 236
vs_OESSAO_source_glsl410:
	.string	"#version 410\n\nlayout(location = 0) out vec2 uv;\nlayout(location = 1) in vec2 OESSAO_texcoord;\nlayout(location = 0) in vec2 OESSAO_position;\n\nvoid main()\n{\n    uv = OESSAO_texcoord;\n    gl_Position = vec4(OESSAO_position, 0.0, 1.0);\n}\n\n"
	.align 32
	.type	fs_OESSAO_source_glsl410, @object
	.size	fs_OESSAO_source_glsl410, 2579
fs_OESSAO_source_glsl410:
	.ascii	"#version 410\n\nuniform vec4 OESSAO_params[4];\nuniform samp"
	.ascii	"ler2D OESSAO_texture_OESSAO_smp;\nuniform sampler2D OESSAO_d"
	.ascii	"texture_OESSAO_smp;\nuniform sampler2D OESSAO_ptexture_OESSA"
	.ascii	"O_smp;\nuniform sampler2D OESSAO_ntexture_OESSAO_smp;\n\nlay"
	.ascii	"out(location = 0) in vec2 uv;\nlayout(location = 0) out vec4"
	.ascii	" frag_color;\n\nfloat QISQRT(float _x)\n{\n    float _39 = u"
	.ascii	"intBitsToFloat(1597463007u - (floatBitsToUint(_x) >> uint(1)"
	.ascii	"));\n    return _39 * (1.5 - (((0.5 * _x) * _39) * _39));\n}"
	.ascii	"\n\nfloat QSQRT(float _x)\n{\n    float param = _x;\n    ret"
	.ascii	"urn _x * QISQRT(param);\n}\n\nuint pcg(uint v)\n{\n    uint "
	.ascii	"_65 = (v * 747796405u) + 2891336453u;\n    uint _77 = ((_65 "
	.ascii	">> ((_65 >> 28u) + 4u)) ^ _65) * 277803737u;\n    return (_7"
	.ascii	"7 >> 22u) ^ _77;\n}\n\nfloat getRandom(vec2 p)\n{\n    uint "
	.ascii	"param = uint(p.x);\n    uint param_1 = pcg(param) + uint(p.y"
	.ascii	");\n    return float(pcg(param_1)) * 2.328306436538696289062"
	.ascii	"5e-10;\n}\n\nvoid main()\n{\n    vec4 _122 = texture(OESSAO_"
	.ascii	"texture_OESSAO_smp, uv);\n    vec3 _123 = _122.xyz;\n    vec"
	.ascii	"4 _130 = texture(OESSAO_dtexture_OESSAO_smp, uv);\n    float"
	.ascii	" _132 = _130.z;\n    vec4 _139 = texture(OESSAO_ptexture_OES"
	.ascii	"SAO_smp, uv);\n    vec3 _140 = _139.xyz;\n    float param = "
	.ascii	"dot(_140, _140);\n    if (QSQRT(param) < 0.00100000004749745"
	.ascii	"1305389404296875)\n    {\n        frag_color = vec4(_122.xyz"
	.ascii	", 1.0);\n        return;\n    }\n    vec3 _171 = (texture(OE"
	.ascii	"SSAO_ntexture_OESSAO_smp, uv).xyz * 2.0) - vec3(1.0);\n    b"
	.ascii	"vec3 _193 = bvec3(abs(_171.z) < 0.999000012874603271484375);"
	.ascii	"\n    vec3 _198 = cross(vec3(_193.x ? vec3(0.0, 0.0, 1.0).x "
	.ascii	": vec3(1.0, 0.0, 0.0).x, _193.y ? vec3(0.0, 0.0, 1.0).y : ve"
	.ascii	"c3(1.0, 0.0, 0.0).y, _193.z ? vec3(0.0, 0.0, 1.0).z : vec3(1"
	.ascii	".0, 0.0, 0.0).z), _171);\n    float hits = 0.349999994039535"
	.ascii	"5224609375;\n    vec2 param_1 = vec2(0.0, 2026.0);\n    floa"
	.ascii	"t _232 = 6.28318500518798828125 * getRandom(param_1);\n    f"
	.ascii	"loat param_2 = 0.839999973773956298828125;\n    float _246 ="
	.ascii	" 0.839999973773956298828125 * QISQRT(param_2);\n    vec3 _27"
	.ascii	"0 = _140 + (((mat3(_198, cross(_171, _198), _171) * vec3(cos"
	.ascii	"(_232) * _246, sin(_232) * _246, 0.4000000059604644775390625"
	.ascii	")) * vec3(0.2857142984867095947265625)) * mix(0.100000001490"
	.ascii	"116119384765625, 0.60000002384185791015625, smoothstep(0.100"
	.ascii	"000001490116119384765625, 5.0, _132)));\n    vec4 _286 = mat"
	.ascii	"4(OESSAO_params[0], OESSAO_params[1], OESSAO_params[2], OESS"
	.ascii	"AO_params[3]) * vec4(_270, 1.0);\n    if (t"
	.string	"exture(OESSAO_ptexture_OESSAO_smp, ((_286.xy / vec2(_286.w)) * 0.5) + vec2(0.5)).z < (_270.z - (0.00200000009499490261077880859375 * _132)))\n    {\n        hits = 1.0;\n    }\n    frag_color = vec4((_123 + (_123 * 0.60000002384185791015625)) * hits, 1.0);\n}\n\n"
	.align 32
	.type	vs_OERayTracer_source_glsl410, @object
	.size	vs_OERayTracer_source_glsl410, 764
vs_OERayTracer_source_glsl410:
	.ascii	"#version 410\n\nuniform vec4 rtvs_params[8];\nlayout(locatio"
	.ascii	"n = 0) in vec3 position;\nlayout(location = 0) out vec4 colo"
	.ascii	"r;\nlayout(location = 1) in vec4 color0;\nlayout(location = "
	.ascii	"1) out vec3 normal;\nlayout(location = 2) in vec3 normal0;\n"
	.ascii	"layout(location = 2) out vec3 fragPos;\nlayout(location = 3)"
	.ascii	" out vec2 texcoord;\nlayout(location = 3) in vec2 texcoord0;"
	.ascii	"\n\nvoid main()\n{\n    vec4 _31 = vec4(position, 1.0);\n   "
	.ascii	" gl_Position = mat4(rtvs_params[0], rtvs_params[1], rtvs_par"
	.ascii	"ams[2], rtvs_params[3]) * _31;\n    color ="
	.string	" color0;\n    mat4 _44 = mat4(rtvs_params[4], rtvs_params[5], rtvs_params[6], rtvs_params[7]);\n    normal = normalize(transpose(inverse(mat3(_44[0].xyz, _44[1].xyz, _44[2].xyz))) * normal0);\n    fragPos = vec3((_44 * _31).xyz);\n    texcoord = texcoord0;\n}\n\n"
	.align 32
	.type	fs_OERayTracer_source_glsl410, @object
	.size	fs_OERayTracer_source_glsl410, 460
fs_OERayTracer_source_glsl410:
	.ascii	"#version 410\n\nuniform vec4 rtlight_params[128];\nuniform v"
	.ascii	"ec4 rtfs_params[1];\nuniform sampler2D _texture_smp;\n\nlayo"
	.ascii	"ut(location = 3) in vec2 texcoord;\nlayout(location = 0) out"
	.ascii	" vec4 frag_color;\nlayout(locat"
	.string	"ion = 0) in vec4 color;\nlayout(location = 1) in vec3 normal;\nlayout(location = 2) in vec3 fragPos;\n\nvoid main()\n{\n    frag_color = vec4(texture(_texture_smp, texcoord).xyz * ((rtfs_params[0].xyz * rtlight_params[64].xyz) * rtlight_params[0].xyz), 1.0);\n}\n\n"
	.align 32
	.type	vs_OESSGI_source_glsl410, @object
	.size	vs_OESSGI_source_glsl410, 236
vs_OESSGI_source_glsl410:
	.string	"#version 410\n\nlayout(location = 0) out vec2 uv;\nlayout(location = 1) in vec2 OESSGI_texcoord;\nlayout(location = 0) in vec2 OESSGI_position;\n\nvoid main()\n{\n    uv = OESSGI_texcoord;\n    gl_Position = vec4(OESSGI_position, 0.0, 1.0);\n}\n\n"
	.align 32
	.type	fs_OESSGI_source_glsl410, @object
	.size	fs_OESSGI_source_glsl410, 4154
fs_OESSGI_source_glsl410:
	.ascii	"#version 410\n\nstruct OESSGI_params\n{\n    mat4 proj;\n   "
	.ascii	" int RAYS;\n    int STEPS;\n};\n\nuniform OESSGI_params _282"
	.ascii	";\n\nuniform sampler2D OESSGI_texture_OESSGI_smp;\nuniform s"
	.ascii	"ampler2D OESSGI_ptexture_OESSGI_smp;\nuniform sampler2D OESS"
	.ascii	"GI_ntexture_OESSGI_smp;\n\nlayout(location = 0) in vec2 uv;\n"
	.ascii	"layout(location = 0) out vec4 frag_color;\n\nfloat QISQRT(fl"
	.ascii	"oat _x)\n{\n    float _45 = uintBitsToFloat(1597463007u - (f"
	.ascii	"loatBitsToUint(_x) >> uint(1)));\n    return _45 * (1.5 - (("
	.ascii	"(0.5 * _x) * _45) * _45));\n}\n\nfloat QSQRT(float _x)\n{\n "
	.ascii	"   float param = _x;\n    return _x * QISQRT(param);\n}\n\nu"
	.ascii	"int pcg(uint v)\n{\n    uint _143 = (v * 747796405u) + 28913"
	.ascii	"36453u;\n    uint _155 = ((_143 >> ((_143 >> 28u) + 4u)) ^ _"
	.ascii	"143) * 277803737u;\n    return (_155 >> 22u) ^ _155;\n}\n\nf"
	.ascii	"loat getRandom(vec2 p)\n{\n    uint param = uint(p.x);\n    "
	.ascii	"uint param_1 = pcg(param) + uint(p.y);\n    return float(pcg"
	.ascii	"(param_1)) * 2.3283064365386962890625e-10;\n}\n\nvec3 WNORM("
	.ascii	"vec3 _x)\n{\n    float param = ((_x.x * _x.x) + (_x.y * _x.y"
	.ascii	")) + (_x.z * _x.z);\n    float param_1 = ((_x.x * _x.x) + (_"
	.ascii	"x.y * _x.y)) + (_x.z * _x.z);\n    float param_2 = ((_x.x * "
	.ascii	"_x.x) + (_x.y * _x.y)) + (_x.z * _x.z);\n    return vec3(_x."
	.ascii	"x * QISQRT(param), _x.y * QISQRT(param_1), _x.z * QISQRT(par"
	.ascii	"am_2));\n}\n\nvoid main()\n{\n    vec4 _196 = texture(OESSGI"
	.ascii	"_texture_OESSGI_smp, uv);\n    vec4 _204 = texture(OESSGI_pt"
	.ascii	"exture_OESSGI_smp, uv);\n    vec3 _205 = _204.xyz;\n    floa"
	.ascii	"t param = dot(_205, _205);\n    if (QSQRT(param) < 0.0010000"
	.ascii	"00047497451305389404296875)\n    {\n        frag_color = vec"
	.ascii	"4(_196.xyz, 1.0);\n        return;\n    }\n    vec4 _231 = t"
	.ascii	"exture(OESSGI_ntexture_OESSGI_smp, uv);\n    vec3 _236 = (_2"
	.ascii	"31.xyz * 2.0) - vec3(1.0);\n    bvec3 _247 = bvec3(abs(_236."
	.ascii	"z) < 0.999000012874603271484375);\n    vec3 _252 = cross(vec"
	.ascii	"3(_247.x ? vec3(0.0, 0.0, 1.0).x : vec3(1.0, 0.0, 0.0).x, _2"
	.ascii	"47.y ? vec3(0.0, 0.0, 1.0).y : vec3(1.0, 0.0, 0.0).y, _247.z"
	.ascii	" ? vec3(0.0, 0.0, 1.0).z : vec3(1.0, 0.0, 0.0).z), _236);\n "
	.ascii	"   mat3 _275 = mat3(_252, cross(_236, _252), _236);\n    vec"
	.ascii	"3 GI = vec3(0.0);\n    float _288 = 1.0 / float(_282.STEPS);"
	.ascii	"\n    int i = 0;\n    int hits = 0;\n    int _302;\n    for "
	.ascii	"(;;)\n    {\n        _302 = _282.RAYS;\n        if (i < _302"
	.ascii	")\n        {\n            vec2 param_1 = vec2(float(i), 2025"
	.ascii	".0);\n            float _312 = 6.28318500518798828125 * getR"
	.ascii	"andom(param_1);\n            vec2 param_2 = vec2(2025.0, flo"
	.ascii	"at(i));\n            float _318 = getRandom(param_2);\n     "
	.ascii	"       float _323 = 1.0 - (_318 * _318);\n            float "
	.ascii	"param_3 = _323;\n            float _330 = _323 * QISQRT(para"
	.ascii	"m_3);\n            vec3 param_4 = _275 * vec3(cos(_312) * _3"
	.ascii	"30, sin(_312) * _330, _318);\n            vec3 _347 = WNORM("
	.ascii	"param_4);\n            for (int j = 1; j <= _282.STEPS; j++)"
	.ascii	"\n            {\n                vec3 _370 = _205 + (_347 * "
	.ascii	"((float(j) * _288) * 0.25));\n                vec4 _381 = _2"
	.ascii	"82.proj * vec4(_370, 1.0);\n                vec2 _392 = ((_3"
	.ascii	"81.xy / vec2(_381.w)) * 0.5) + vec2(0.5);\n                b"
	.ascii	"ool _397 = any(lessThan(_392, vec2(0.0)));\n                "
	.ascii	"bool _405;\n                if (!_397)\n                {\n "
	.ascii	"                   _405 = any(greaterThan(_392, vec2(1.0)));"
	.ascii	"\n                }\n                else\n                {"
	.ascii	"\n                    _405 = _397;\n                }\n     "
	.ascii	"           if (_405)\n                {\n                   "
	.ascii	" continue;\n                }\n                vec4 _414 = t"
	.ascii	"exture(OESSGI_ptexture_OESSGI_smp, _392);\n                v"
	.ascii	"ec3 _415 = _414.xyz;\n                float param_5 = dot(_4"
	.ascii	"15, _415);\n                if (QSQRT(param_5) < 0.001000000"
	.ascii	"047497451305389404296875)\n                {\n              "
	.ascii	"      continue;\n                }\n                if (_414"
	.ascii	".z < (_370.z - 0.0500000007450580596923828125))\n           "
	.ascii	"     {\n                    GI += (texture(OESSGI_texture_OE"
	.ascii	"SSGI_smp, _392).xyz * 0.4000000059604644775390625);\n       "
	.ascii	"             hits++;\n                    break;\n          "
	.ascii	"      }\n            }\n            i++;\n            con"
	.string	"tinue;\n        }\n        else\n        {\n            break;\n        }\n    }\n    vec3 _453 = GI;\n    float _456 = float(_302);\n    vec3 _459 = (_453 / vec3(_456)) * 2.0;\n    GI = _459;\n    frag_color = vec4((_196.xyz + _459) * (float(hits) / _456), 1.0);\n}\n\n"
	.align 32
	.type	vs_OEDNOISE_source_glsl410, @object
	.size	vs_OEDNOISE_source_glsl410, 244
vs_OEDNOISE_source_glsl410:
	.string	"#version 410\n\nlayout(location = 0) out vec2 uv;\nlayout(location = 1) in vec2 OEDNOISE_texcoord;\nlayout(location = 0) in vec2 OEDNOISE_position;\n\nvoid main()\n{\n    uv = OEDNOISE_texcoord;\n    gl_Position = vec4(OEDNOISE_position, 0.0, 1.0);\n}\n\n"
	.align 32
	.type	fs_OEDNOISE_source_glsl410, @object
	.size	fs_OEDNOISE_source_glsl410, 1149
fs_OEDNOISE_source_glsl410:
	.ascii	"#version 410\n\nuniform vec4 OEDNOISE_params[1];\nuniform sa"
	.ascii	"mpler2D OEDNOISE_texture_OEDNOISE_smp;\n\nlayout(location = "
	.ascii	"0) in vec2 uv;\nlayout(location = 0) out vec4 frag_color;\n\n"
	.ascii	"void main()\n{\n    vec4 _24 = texture(OEDNOISE_texture_OEDN"
	.ascii	"OISE_smp, uv);\n    float zBuff = 0.0;\n    vec4 aBuff = vec"
	.ascii	"4(0.0);\n    vec2 d;\n    vec2 _150 = d;\n    _150.x = -4.0;"
	.ascii	"\n    d = _150;\n    vec2 _161;\n    vec2 _163;\n    for (; "
	.ascii	"d.x <= 4.0; _161 = d, _163 = _161, _163.x = _161.x + 1.0, d "
	.ascii	"= _163)\n    {\n        vec2 _153 = d;\n        float _69 = "
	.ascii	"sqrt(16.0 - (_153.x * _153.x));\n        vec2 _156 = _153;\n"
	.ascii	"        _156.y = -_69;\n        d = _156;\n        vec2 _158"
	.ascii	";\n        vec2 _160;\n        for (; d.y <= _69; _158 = d, "
	.ascii	"_160 = _158, _160.y = _158.y + 1.0, d = _160)\n        {\n  "
	.ascii	"          vec4 _109 = texture(OEDNOISE_texture_OEDNOISE_smp,"
	.ascii	" uv + (d / OEDNOISE_params[0].xy));\n            vec3 _117 ="
	.ascii	" (_109 - _24).xyz;\n   "
	.string	"         float _128 = (exp(dot(_117, _117) * (-50.0)) * 3.98942279815673828125) * (exp(dot(d, d) * (-0.125)) * 3.18309879302978515625);\n            zBuff += _128;\n            aBuff += (_109 * _128);\n        }\n    }\n    frag_color = aBuff / vec4(zBuff);\n}\n\n"
	.local	globalRenderer
	.comm	globalRenderer,8,8
	.align 8
.LC0:
	.string	"View is currently initializing [SG_RESOURCESTATE_INITIAL]"
.LC1:
	.string	"[%d]: %s"
	.align 8
.LC2:
	.string	"View is currently allocating [SG_RESOURCESTATE_ALLOC]"
	.align 8
.LC3:
	.string	"View is valid and ready [SG_RESOURCESTATE_VALID]"
	.align 8
.LC4:
	.string	"View has failed initialization [SG_RESOURCESTATE_FAILED]"
	.align 8
.LC5:
	.string	"View is invalid [SG_RESOURCESTATE_INVALID]"
	.align 8
.LC6:
	.string	"/home/wt/Projects/obliviengine/include/OE/OE.h"
.LC7:
	.string	"12:49:00"
.LC8:
	.string	"%s VIEW_INFO  (%s:%d): %s\n"
	.text
	.type	OECheckViewState, @function
OECheckViewState:
.LFB598:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movl	%edi, -36(%rbp)
	movl	-36(%rbp), %eax
	movl	%eax, %edi
	call	sg_query_view_state
	movl	%eax, -12(%rbp)
	cmpl	$2147483647, -12(%rbp)
	je	.L10
	cmpl	$2147483647, -12(%rbp)
	ja	.L3
	cmpl	$4, -12(%rbp)
	je	.L4
	cmpl	$4, -12(%rbp)
	ja	.L3
	cmpl	$3, -12(%rbp)
	je	.L5
	cmpl	$3, -12(%rbp)
	ja	.L3
	cmpl	$2, -12(%rbp)
	je	.L6
	cmpl	$2, -12(%rbp)
	ja	.L3
	cmpl	$0, -12(%rbp)
	je	.L7
	cmpl	$1, -12(%rbp)
	je	.L8
	jmp	.L3
.L7:
	movq	$.LC0, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	strlen
	subq	$-128, %rax
	movq	%rax, -32(%rbp)
	movq	-32(%rbp), %rax
	movl	$1, %esi
	movq	%rax, %rdi
	call	calloc
	movq	%rax, -8(%rbp)
	movl	-36(%rbp), %edx
	movq	-24(%rbp), %rcx
	movq	-32(%rbp), %rsi
	movq	-8(%rbp), %rax
	movq	%rcx, %r8
	movl	%edx, %ecx
	movl	$.LC1, %edx
	movq	%rax, %rdi
	movl	$0, %eax
	call	snprintf
	jmp	.L3
.L8:
	movq	$.LC2, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	strlen
	subq	$-128, %rax
	movq	%rax, -32(%rbp)
	movq	-32(%rbp), %rax
	movl	$1, %esi
	movq	%rax, %rdi
	call	calloc
	movq	%rax, -8(%rbp)
	movl	-36(%rbp), %edx
	movq	-24(%rbp), %rcx
	movq	-32(%rbp), %rsi
	movq	-8(%rbp), %rax
	movq	%rcx, %r8
	movl	%edx, %ecx
	movl	$.LC1, %edx
	movq	%rax, %rdi
	movl	$0, %eax
	call	snprintf
	jmp	.L3
.L6:
	movq	$.LC3, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	strlen
	subq	$-128, %rax
	movq	%rax, -32(%rbp)
	movq	-32(%rbp), %rax
	movl	$1, %esi
	movq	%rax, %rdi
	call	calloc
	movq	%rax, -8(%rbp)
	movl	-36(%rbp), %edx
	movq	-24(%rbp), %rcx
	movq	-32(%rbp), %rsi
	movq	-8(%rbp), %rax
	movq	%rcx, %r8
	movl	%edx, %ecx
	movl	$.LC1, %edx
	movq	%rax, %rdi
	movl	$0, %eax
	call	snprintf
	jmp	.L3
.L5:
	movq	$.LC4, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	strlen
	subq	$-128, %rax
	movq	%rax, -32(%rbp)
	movq	-32(%rbp), %rax
	movl	$1, %esi
	movq	%rax, %rdi
	call	calloc
	movq	%rax, -8(%rbp)
	movl	-36(%rbp), %edx
	movq	-24(%rbp), %rcx
	movq	-32(%rbp), %rsi
	movq	-8(%rbp), %rax
	movq	%rcx, %r8
	movl	%edx, %ecx
	movl	$.LC1, %edx
	movq	%rax, %rdi
	movl	$0, %eax
	call	snprintf
	jmp	.L3
.L4:
	movq	$.LC5, -24(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	strlen
	subq	$-128, %rax
	movq	%rax, -32(%rbp)
	movq	-32(%rbp), %rax
	movl	$1, %esi
	movq	%rax, %rdi
	call	calloc
	movq	%rax, -8(%rbp)
	movl	-36(%rbp), %edx
	movq	-24(%rbp), %rcx
	movq	-32(%rbp), %rsi
	movq	-8(%rbp), %rax
	movq	%rcx, %r8
	movl	%edx, %ecx
	movl	$.LC1, %edx
	movq	%rax, %rdi
	movl	$0, %eax
	call	snprintf
.L10:
	nop
.L3:
	cmpq	$0, -8(%rbp)
	je	.L11
	movq	stdout(%rip), %rax
	movq	-8(%rbp), %rdx
	movq	%rdx, %r9
	movl	$838, %r8d
	movl	$.LC6, %ecx
	movl	$.LC7, %edx
	movl	$.LC8, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf
	movq	-8(%rbp), %rax
	movq	%rax, %rdi
	call	free
.L11:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE598:
	.size	OECheckViewState, .-OECheckViewState
	.type	OECheckOEViews, @function
OECheckOEViews:
.LFB599:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	cmpq	$0, -8(%rbp)
	je	.L14
	movq	-8(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, %edi
	call	OECheckViewState
	movq	-8(%rbp), %rax
	movl	4(%rax), %eax
	movl	%eax, %edi
	call	OECheckViewState
	movq	-8(%rbp), %rax
	movl	8(%rax), %eax
	movl	%eax, %edi
	call	OECheckViewState
	movq	-8(%rbp), %rax
	movl	12(%rax), %eax
	movl	%eax, %edi
	call	OECheckViewState
	movq	-8(%rbp), %rax
	movl	16(%rax), %eax
	movl	%eax, %edi
	call	OECheckViewState
	movq	-8(%rbp), %rax
	movl	20(%rax), %eax
	movl	%eax, %edi
	call	OECheckViewState
	movq	-8(%rbp), %rax
	movl	24(%rax), %eax
	movl	%eax, %edi
	call	OECheckViewState
	movq	-8(%rbp), %rax
	movl	28(%rax), %eax
	movl	%eax, %edi
	call	OECheckViewState
	movq	-8(%rbp), %rax
	movl	32(%rax), %eax
	movl	%eax, %edi
	call	OECheckViewState
	movq	-8(%rbp), %rax
	movl	36(%rax), %eax
	movl	%eax, %edi
	call	OECheckViewState
	movq	-8(%rbp), %rax
	movl	40(%rax), %eax
	movl	%eax, %edi
	call	OECheckViewState
	movq	-8(%rbp), %rax
	movl	44(%rax), %eax
	movl	%eax, %edi
	call	OECheckViewState
	movq	-8(%rbp), %rax
	movl	48(%rax), %eax
	movl	%eax, %edi
	call	OECheckViewState
	movq	-8(%rbp), %rax
	movl	52(%rax), %eax
	movl	%eax, %edi
	call	OECheckViewState
	movq	-8(%rbp), %rax
	movl	56(%rax), %eax
	movl	%eax, %edi
	call	OECheckViewState
	movq	-8(%rbp), %rax
	movl	60(%rax), %eax
	movl	%eax, %edi
	call	OECheckViewState
.L14:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE599:
	.size	OECheckOEViews, .-OECheckOEViews
	.type	OEDestroyViews, @function
OEDestroyViews:
.LFB600:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	cmpq	$0, -8(%rbp)
	je	.L17
	movq	-8(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, %edi
	call	sg_destroy_view
	movq	-8(%rbp), %rax
	movl	4(%rax), %eax
	movl	%eax, %edi
	call	sg_destroy_view
	movq	-8(%rbp), %rax
	movl	8(%rax), %eax
	movl	%eax, %edi
	call	sg_destroy_view
	movq	-8(%rbp), %rax
	movl	12(%rax), %eax
	movl	%eax, %edi
	call	sg_destroy_view
	movq	-8(%rbp), %rax
	movl	16(%rax), %eax
	movl	%eax, %edi
	call	sg_destroy_view
	movq	-8(%rbp), %rax
	movl	20(%rax), %eax
	movl	%eax, %edi
	call	sg_destroy_view
	movq	-8(%rbp), %rax
	movl	24(%rax), %eax
	movl	%eax, %edi
	call	sg_destroy_view
	movq	-8(%rbp), %rax
	movl	28(%rax), %eax
	movl	%eax, %edi
	call	sg_destroy_view
	movq	-8(%rbp), %rax
	movl	32(%rax), %eax
	movl	%eax, %edi
	call	sg_destroy_view
	movq	-8(%rbp), %rax
	movl	36(%rax), %eax
	movl	%eax, %edi
	call	sg_destroy_view
	movq	-8(%rbp), %rax
	movl	40(%rax), %eax
	movl	%eax, %edi
	call	sg_destroy_view
	movq	-8(%rbp), %rax
	movl	44(%rax), %eax
	movl	%eax, %edi
	call	sg_destroy_view
	movq	-8(%rbp), %rax
	movl	48(%rax), %eax
	movl	%eax, %edi
	call	sg_destroy_view
	movq	-8(%rbp), %rax
	movl	52(%rax), %eax
	movl	%eax, %edi
	call	sg_destroy_view
	movq	-8(%rbp), %rax
	movl	56(%rax), %eax
	movl	%eax, %edi
	call	sg_destroy_view
	movq	-8(%rbp), %rax
	movl	60(%rax), %eax
	movl	%eax, %edi
	call	sg_destroy_view
.L17:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE600:
	.size	OEDestroyViews, .-OEDestroyViews
	.type	OECheckScreenFlag, @function
OECheckScreenFlag:
.LFB601:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)
	cmpl	$3, -4(%rbp)
	je	.L19
	cmpl	$5, -4(%rbp)
	je	.L19
	cmpl	$9, -4(%rbp)
	je	.L19
	cmpl	$17, -4(%rbp)
	jne	.L20
.L19:
	movl	$1, %eax
	jmp	.L21
.L20:
	movl	$0, %eax
.L21:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE601:
	.size	OECheckScreenFlag, .-OECheckScreenFlag
	.type	OECheckGraphicFlag, @function
OECheckGraphicFlag:
.LFB602:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, -4(%rbp)
	cmpl	$65, -4(%rbp)
	je	.L23
	cmpl	$129, -4(%rbp)
	je	.L23
	cmpl	$257, -4(%rbp)
	jne	.L24
.L23:
	movl	$1, %eax
	jmp	.L25
.L24:
	movl	$0, %eax
.L25:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE602:
	.size	OECheckGraphicFlag, .-OECheckGraphicFlag
	.section	.rodata
.LC9:
	.string	"Failed to init threadpool!"
	.text
	.type	OEInitThreadPool, @function
OEInitThreadPool:
.LFB603:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	movl	$0, %edx
	movl	$65536, %esi
	movl	$1, %edi
	call	threadpool_create
	movq	-8(%rbp), %rdx
	movq	%rax, (%rdx)
	cmpq	$0, -8(%rbp)
	jne	.L28
	movq	stderr(%rip), %rax
	movq	%rax, %rcx
	movl	$26, %edx
	movl	$1, %esi
	movl	$.LC9, %edi
	call	fwrite
	movl	$1, %edi
	call	exit
.L28:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE603:
	.size	OEInitThreadPool, .-OEInitThreadPool
	.section	.rodata
.LC10:
	.string	"Failed to dispatch thread!"
	.align 8
.LC11:
	.string	"Thread successfully dispatched"
.LC12:
	.string	"%s INFO_THREAD  (%s:%d): %s\n"
	.text
	.type	OEDispatchThread, @function
OEDispatchThread:
.LFB604:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	%rcx, -32(%rbp)
	cmpq	$0, -8(%rbp)
	je	.L34
	cmpq	$0, -16(%rbp)
	je	.L34
	movq	-32(%rbp), %rcx
	movq	-24(%rbp), %rdx
	movq	-16(%rbp), %rsi
	movq	-8(%rbp), %rax
	movq	%rcx, %r8
	movl	$0, %ecx
	movq	%rax, %rdi
	call	threadpool_add
	testl	%eax, %eax
	je	.L33
	movq	stderr(%rip), %rax
	movq	%rax, %rcx
	movl	$26, %edx
	movl	$1, %esi
	movl	$.LC10, %edi
	call	fwrite
	movl	$1, %edi
	call	exit
.L33:
	movq	stdout(%rip), %rax
	movl	$.LC11, %r9d
	movl	$911, %r8d
	movl	$.LC6, %ecx
	movl	$.LC7, %edx
	movl	$.LC12, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf
	jmp	.L29
.L34:
	nop
.L29:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE604:
	.size	OEDispatchThread, .-OEDispatchThread
	.type	OEGetThreadState, @function
OEGetThreadState:
.LFB605:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	cmpq	$0, -24(%rbp)
	je	.L36
	cmpq	$0, -32(%rbp)
	jne	.L37
.L36:
	movl	$-1, %eax
	jmp	.L38
.L37:
	movl	$-1, -8(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	pthread_mutex_lock
	movl	$0, -4(%rbp)
	jmp	.L39
.L41:
	addl	$1, -4(%rbp)
.L39:
	movq	-24(%rbp), %rax
	movl	104(%rax), %eax
	cmpl	%eax, -4(%rbp)
	jge	.L40
	movq	-24(%rbp), %rax
	movq	96(%rax), %rax
	movl	-4(%rbp), %edx
	movslq	%edx, %rdx
	salq	$5, %rdx
	addq	%rdx, %rax
	movq	24(%rax), %rax
	movq	-32(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	strcmp
	testl	%eax, %eax
	jne	.L41
.L40:
	movq	-24(%rbp), %rax
	movq	96(%rax), %rax
	movl	-4(%rbp), %edx
	movslq	%edx, %rdx
	salq	$5, %rdx
	addq	%rdx, %rax
	movzbl	16(%rax), %eax
	sall	$4, %eax
	sarb	$4, %al
	movsbl	%al, %eax
	movl	%eax, -8(%rbp)
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	pthread_mutex_unlock
	movl	-8(%rbp), %eax
.L38:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE605:
	.size	OEGetThreadState, .-OEGetThreadState
	.type	OESetThreadState, @function
OESetThreadState:
.LFB606:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movl	%edx, -36(%rbp)
	cmpq	$0, -24(%rbp)
	je	.L49
	cmpq	$0, -32(%rbp)
	je	.L49
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	pthread_mutex_lock
	movl	$0, -4(%rbp)
	jmp	.L46
.L48:
	addl	$1, -4(%rbp)
.L46:
	movq	-24(%rbp), %rax
	movl	104(%rax), %eax
	cmpl	%eax, -4(%rbp)
	jge	.L47
	movq	-24(%rbp), %rax
	movq	96(%rax), %rax
	movl	-4(%rbp), %edx
	movslq	%edx, %rdx
	salq	$5, %rdx
	addq	%rdx, %rax
	movq	24(%rax), %rax
	movq	-32(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	strcmp
	testl	%eax, %eax
	jne	.L48
.L47:
	movq	-24(%rbp), %rax
	movq	96(%rax), %rax
	movl	-4(%rbp), %edx
	movslq	%edx, %rdx
	salq	$5, %rdx
	addq	%rax, %rdx
	movl	-36(%rbp), %eax
	sall	$4, %eax
	sarb	$4, %al
	andl	$15, %eax
	movl	%eax, %ecx
	movzbl	16(%rdx), %eax
	andl	$-16, %eax
	orl	%ecx, %eax
	movb	%al, 16(%rdx)
	movq	-24(%rbp), %rax
	addq	$40, %rax
	movq	%rax, %rdi
	call	pthread_cond_broadcast
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	pthread_mutex_unlock
	jmp	.L42
.L49:
	nop
.L42:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE606:
	.size	OESetThreadState, .-OESetThreadState
	.type	OEWaitThread, @function
OEWaitThread:
.LFB607:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movl	%edx, -36(%rbp)
	cmpq	$0, -24(%rbp)
	je	.L59
	cmpq	$0, -32(%rbp)
	je	.L59
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	pthread_mutex_lock
	movl	$0, -4(%rbp)
	jmp	.L54
.L56:
	addl	$1, -4(%rbp)
.L54:
	movq	-24(%rbp), %rax
	movl	104(%rax), %eax
	cmpl	%eax, -4(%rbp)
	jge	.L57
	movq	-24(%rbp), %rax
	movq	96(%rax), %rax
	movl	-4(%rbp), %edx
	movslq	%edx, %rdx
	salq	$5, %rdx
	addq	%rdx, %rax
	movq	24(%rax), %rax
	movq	-32(%rbp), %rdx
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	strcmp
	testl	%eax, %eax
	jne	.L56
	jmp	.L57
.L58:
	movq	-24(%rbp), %rax
	movq	-24(%rbp), %rdx
	addq	$40, %rdx
	movq	%rax, %rsi
	movq	%rdx, %rdi
	call	pthread_cond_wait
.L57:
	movq	-24(%rbp), %rax
	movq	96(%rax), %rax
	movl	-4(%rbp), %edx
	movslq	%edx, %rdx
	salq	$5, %rdx
	addq	%rdx, %rax
	movzbl	16(%rax), %eax
	sall	$4, %eax
	sarb	$4, %al
	movsbl	%al, %eax
	cmpl	%eax, -36(%rbp)
	jne	.L58
	movq	-24(%rbp), %rax
	movq	%rax, %rdi
	call	pthread_mutex_unlock
	jmp	.L50
.L59:
	nop
.L50:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE607:
	.size	OEWaitThread, .-OEWaitThread
	.type	OEDestroyThreadPool, @function
OEDestroyThreadPool:
.LFB608:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%rdi, -8(%rbp)
	cmpq	$0, -8(%rbp)
	je	.L63
	movq	-8(%rbp), %rax
	movl	$0, %esi
	movq	%rax, %rdi
	call	threadpool_destroy
	jmp	.L60
.L63:
	nop
.L60:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE608:
	.size	OEDestroyThreadPool, .-OEDestroyThreadPool
	.section	.rodata
.LC13:
	.string	"OECube"
	.text
	.globl	draw
	.type	draw, @function
draw:
.LFB609:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$.LC13, %edi
	call	OEGetObjectFromName
	movq	%rax, %rdi
	call	OEDrawObject
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE609:
	.size	draw, .-draw
	.globl	event
	.type	event, @function
event:
.LFB610:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE610:
	.size	event, .-event
	.section	.rodata
.LC14:
	.string	"game"
	.text
	.globl	main
	.type	main, @function
main:
.LFB611:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movq	%rsi, -16(%rbp)
	movl	$0, %ecx
	movl	$.LC14, %edx
	movl	$720, %esi
	movl	$1280, %edi
	call	OEInitRenderer
	call	OEEnableDebugInfo
	call	OEEnableFXAA
	movl	$8, %esi
	movl	$64, %edi
	call	OEEnableSSGI
	jmp	.L67
.L68:
	movl	$event, %edi
	call	OEPollEvents
	movl	$0, %edx
	movl	$0, %esi
	movl	$draw, %edi
	call	OERenderFrame
.L67:
	call	OERendererIsRunning
	testl	%eax, %eax
	jne	.L68
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE611:
	.size	main, .-main
	.ident	"GCC: (GNU) 15.2.1 20251111 (Red Hat 15.2.1-4)"
	.section	.note.GNU-stack,"",@progbits
