/*
 * Copyright (c) 2024 Zhangqi Li (@zhangqili)
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "main.h"
#include "keyboard.h"
#include "rgb.h"
#include "fezui.h"
#include "fezui_var.h"
#include "lfs.h"
#include "usbd_user.h"
#ifdef CONFIG_CHERRYUSB
#else
#include "ch32v30x_usbhs_device.h"
#endif

const uint16_t g_default_keymap[LAYER_NUM][ADVANCED_KEY_NUM + KEY_NUM] = {
    {
        KEYBINDING(KEY_Z, KEY_NO_MODIFIER),
        KEYBINDING(KEY_X, KEY_NO_MODIFIER),
        KEYBINDING(KEY_C, KEY_NO_MODIFIER),
        KEYBINDING(KEY_V, KEY_NO_MODIFIER),
        KEYBINDING(KEY_F8, KEY_NO_MODIFIER),
        KEYBINDING(KEY_F12, KEY_NO_MODIFIER),
        KEYBINDING(KEY_F2, KEY_LEFT_SHIFT),
        KEYBINDING(KEY_F2, KEY_NO_MODIFIER),
        LAYER(LAYER_TOGGLE, 2),
        LAYER(LAYER_TOGGLE, 1),
        KEYBINDING(KEY_ESC, KEY_NO_MODIFIER),
        KEYBINDING(MOUSE_COLLECTION, MOUSE_WHEEL_UP),
        KEYBINDING(MOUSE_COLLECTION, MOUSE_WHEEL_DOWN),
    },
    {
        KEY_TRANSPARENT,
        KEY_TRANSPARENT,
        KEY_TRANSPARENT,
        KEY_TRANSPARENT,
        KEY_TRANSPARENT,
        KEY_TRANSPARENT,
        KEY_TRANSPARENT,
        KEY_TRANSPARENT,
        KEY_USER | (USER_OPENMENU << 8),
        LAYER(LAYER_TOGGLE, 1),
        KEY_TRANSPARENT,
        KEY_TRANSPARENT,
        KEY_TRANSPARENT,
    },
    {
        KEY_TRANSPARENT,
        KEY_TRANSPARENT,
        KEY_TRANSPARENT,
        KEY_TRANSPARENT,
        KEY_TRANSPARENT,
        KEY_TRANSPARENT,
        KEY_TRANSPARENT,
        KEY_TRANSPARENT,
        LAYER(LAYER_TOGGLE, 2),
        KEY_USER | (USER_OPENMENU << 8),
        KEY_TRANSPARENT,
        KEY_TRANSPARENT,
        KEY_TRANSPARENT,
    },
};
const uint8_t g_rgb_mapping[ADVANCED_KEY_NUM] = {0, 1, 2, 3};
const RGBLocation g_rgb_locations[RGB_NUM] = {{0, 0}, {1, 0}, {2, 0}, {3, 0}};
const uint8_t command_advanced_key_mapping[] = {0, 1, 2, 3};
AdvancedKey g_keyboard_advanced_keys[ADVANCED_KEY_NUM] =
{
    {.key.id = 0},
    {.key.id = 1},
    {.key.id = 2},
    {.key.id = 3},
};

Key g_keyboard_keys[KEY_NUM] =
{
    {.id = 4},
    {.id = 5},
    {.id = 6},
    {.id = 7},
    {.id = 8},
    {.id = 9},
    {.id = 10},
    {.id = 11},
    {.id = 12},
};

static const AnalogValue table[] =
{
    -0.002425,
    0.000880,
    0.004171,
    0.007448,
    0.010710,
    0.013957,
    0.017191,
    0.020410,
    0.023614,
    0.026805,
    0.029982,
    0.033145,
    0.036294,
    0.039429,
    0.042550,
    0.045657,
    0.048751,
    0.051831,
    0.054898,
    0.057951,
    0.060991,
    0.064017,
    0.067031,
    0.070030,
    0.073017,
    0.075991,
    0.078951,
    0.081899,
    0.084834,
    0.087756,
    0.090665,
    0.093561,
    0.096445,
    0.099316,
    0.102174,
    0.105020,
    0.107854,
    0.110675,
    0.113483,
    0.116280,
    0.119064,
    0.121836,
    0.124596,
    0.127345,
    0.130081,
    0.132805,
    0.135517,
    0.138217,
    0.140906,
    0.143583,
    0.146248,
    0.148902,
    0.151545,
    0.154175,
    0.156795,
    0.159403,
    0.161999,
    0.164585,
    0.167159,
    0.169722,
    0.172274,
    0.174815,
    0.177345,
    0.179864,
    0.182373,
    0.184870,
    0.187357,
    0.189833,
    0.192298,
    0.194753,
    0.197197,
    0.199630,
    0.202054,
    0.204466,
    0.206869,
    0.209261,
    0.211643,
    0.214015,
    0.216376,
    0.218728,
    0.221069,
    0.223401,
    0.225722,
    0.228034,
    0.230336,
    0.232628,
    0.234910,
    0.237183,
    0.239445,
    0.241699,
    0.243943,
    0.246177,
    0.248402,
    0.250617,
    0.252823,
    0.255020,
    0.257208,
    0.259386,
    0.261556,
    0.263716,
    0.265867,
    0.268009,
    0.270142,
    0.272266,
    0.274381,
    0.276488,
    0.278585,
    0.280674,
    0.282754,
    0.284826,
    0.286889,
    0.288943,
    0.290989,
    0.293027,
    0.295056,
    0.297076,
    0.299089,
    0.301093,
    0.303089,
    0.305076,
    0.307056,
    0.309027,
    0.310990,
    0.312945,
    0.314892,
    0.316832,
    0.318763,
    0.320687,
    0.322602,
    0.324510,
    0.326410,
    0.328303,
    0.330188,
    0.332065,
    0.333935,
    0.335797,
    0.337652,
    0.339499,
    0.341339,
    0.343171,
    0.344996,
    0.346814,
    0.348625,
    0.350428,
    0.352224,
    0.354013,
    0.355795,
    0.357570,
    0.359338,
    0.361100,
    0.362854,
    0.364601,
    0.366341,
    0.368075,
    0.369801,
    0.371522,
    0.373235,
    0.374942,
    0.376642,
    0.378335,
    0.380022,
    0.381702,
    0.383376,
    0.385044,
    0.386705,
    0.388360,
    0.390008,
    0.391650,
    0.393286,
    0.394915,
    0.396539,
    0.398156,
    0.399767,
    0.401372,
    0.402971,
    0.404564,
    0.406151,
    0.407732,
    0.409307,
    0.410877,
    0.412440,
    0.413998,
    0.415550,
    0.417096,
    0.418636,
    0.420171,
    0.421700,
    0.423224,
    0.424741,
    0.426254,
    0.427761,
    0.429262,
    0.430758,
    0.432248,
    0.433734,
    0.435213,
    0.436688,
    0.438157,
    0.439621,
    0.441080,
    0.442533,
    0.443981,
    0.445425,
    0.446863,
    0.448296,
    0.449724,
    0.451147,
    0.452565,
    0.453978,
    0.455386,
    0.456789,
    0.458187,
    0.459581,
    0.460970,
    0.462354,
    0.463733,
    0.465107,
    0.466477,
    0.467842,
    0.469203,
    0.470559,
    0.471910,
    0.473257,
    0.474599,
    0.475937,
    0.477270,
    0.478599,
    0.479924,
    0.481244,
    0.482559,
    0.483871,
    0.485178,
    0.486481,
    0.487779,
    0.489074,
    0.490364,
    0.491650,
    0.492931,
    0.494209,
    0.495483,
    0.496752,
    0.498018,
    0.499279,
    0.500537,
    0.501790,
    0.503040,
    0.504286,
    0.505527,
    0.506765,
    0.507999,
    0.509230,
    0.510456,
    0.511679,
    0.512898,
    0.514113,
    0.515324,
    0.516532,
    0.517736,
    0.518937,
    0.520134,
    0.521327,
    0.522517,
    0.523703,
    0.524886,
    0.526065,
    0.527241,
    0.528413,
    0.529582,
    0.530747,
    0.531909,
    0.533068,
    0.534223,
    0.535376,
    0.536524,
    0.537670,
    0.538812,
    0.539951,
    0.541087,
    0.542219,
    0.543349,
    0.544475,
    0.545598,
    0.546718,
    0.547835,
    0.548949,
    0.550060,
    0.551167,
    0.552272,
    0.553374,
    0.554473,
    0.555568,
    0.556661,
    0.557751,
    0.558838,
    0.559923,
    0.561004,
    0.562082,
    0.563158,
    0.564231,
    0.565301,
    0.566368,
    0.567433,
    0.568495,
    0.569554,
    0.570610,
    0.571664,
    0.572715,
    0.573764,
    0.574809,
    0.575853,
    0.576893,
    0.577931,
    0.578967,
    0.580000,
    0.581030,
    0.582058,
    0.583084,
    0.584107,
    0.585127,
    0.586145,
    0.587161,
    0.588174,
    0.589185,
    0.590193,
    0.591199,
    0.592203,
    0.593204,
    0.594204,
    0.595200,
    0.596195,
    0.597187,
    0.598177,
    0.599165,
    0.600150,
    0.601134,
    0.602115,
    0.603094,
    0.604070,
    0.605045,
    0.606017,
    0.606988,
    0.607956,
    0.608922,
    0.609886,
    0.610848,
    0.611808,
    0.612765,
    0.613721,
    0.614675,
    0.615627,
    0.616577,
    0.617524,
    0.618470,
    0.619414,
    0.620356,
    0.621296,
    0.622234,
    0.623170,
    0.624105,
    0.625037,
    0.625968,
    0.626896,
    0.627823,
    0.628748,
    0.629671,
    0.630593,
    0.631512,
    0.632430,
    0.633346,
    0.634260,
    0.635173,
    0.636083,
    0.636992,
    0.637900,
    0.638805,
    0.639709,
    0.640611,
    0.641512,
    0.642411,
    0.643308,
    0.644203,
    0.645097,
    0.645989,
    0.646880,
    0.647769,
    0.648657,
    0.649543,
    0.650427,
    0.651310,
    0.652191,
    0.653070,
    0.653948,
    0.654825,
    0.655700,
    0.656573,
    0.657445,
    0.658316,
    0.659185,
    0.660052,
    0.660919,
    0.661783,
    0.662646,
    0.663508,
    0.664368,
    0.665227,
    0.666085,
    0.666941,
    0.667795,
    0.668648,
    0.669500,
    0.670351,
    0.671200,
    0.672048,
    0.672894,
    0.673739,
    0.674583,
    0.675425,
    0.676266,
    0.677106,
    0.677944,
    0.678781,
    0.679617,
    0.680452,
    0.681285,
    0.682117,
    0.682948,
    0.683777,
    0.684605,
    0.685432,
    0.686258,
    0.687082,
    0.687905,
    0.688727,
    0.689548,
    0.690367,
    0.691186,
    0.692003,
    0.692819,
    0.693634,
    0.694447,
    0.695259,
    0.696071,
    0.696881,
    0.697690,
    0.698497,
    0.699304,
    0.700109,
    0.700913,
    0.701717,
    0.702519,
    0.703320,
    0.704119,
    0.704918,
    0.705715,
    0.706512,
    0.707307,
    0.708101,
    0.708895,
    0.709687,
    0.710478,
    0.711267,
    0.712056,
    0.712844,
    0.713631,
    0.714416,
    0.715201,
    0.715984,
    0.716767,
    0.717548,
    0.718328,
    0.719108,
    0.719886,
    0.720663,
    0.721439,
    0.722215,
    0.722989,
    0.723762,
    0.724534,
    0.725305,
    0.726075,
    0.726844,
    0.727613,
    0.728380,
    0.729146,
    0.729911,
    0.730675,
    0.731438,
    0.732200,
    0.732962,
    0.733722,
    0.734481,
    0.735239,
    0.735997,
    0.736753,
    0.737508,
    0.738263,
    0.739016,
    0.739769,
    0.740520,
    0.741271,
    0.742020,
    0.742769,
    0.743517,
    0.744263,
    0.745009,
    0.745754,
    0.746498,
    0.747241,
    0.747983,
    0.748724,
    0.749465,
    0.750204,
    0.750942,
    0.751680,
    0.752416,
    0.753152,
    0.753886,
    0.754620,
    0.755353,
    0.756085,
    0.756816,
    0.757546,
    0.758275,
    0.759003,
    0.759731,
    0.760457,
    0.761183,
    0.761907,
    0.762631,
    0.763354,
    0.764075,
    0.764796,
    0.765516,
    0.766236,
    0.766954,
    0.767671,
    0.768388,
    0.769103,
    0.769818,
    0.770532,
    0.771244,
    0.771956,
    0.772667,
    0.773378,
    0.774087,
    0.774795,
    0.775503,
    0.776209,
    0.776915,
    0.777620,
    0.778324,
    0.779027,
    0.779729,
    0.780430,
    0.781130,
    0.781830,
    0.782528,
    0.783226,
    0.783922,
    0.784618,
    0.785313,
    0.786007,
    0.786700,
    0.787393,
    0.788084,
    0.788774,
    0.789464,
    0.790153,
    0.790840,
    0.791527,
    0.792213,
    0.792898,
    0.793582,
    0.794266,
    0.794948,
    0.795629,
    0.796310,
    0.796990,
    0.797668,
    0.798346,
    0.799023,
    0.799699,
    0.800374,
    0.801049,
    0.801722,
    0.802394,
    0.803066,
    0.803737,
    0.804406,
    0.805075,
    0.805743,
    0.806410,
    0.807076,
    0.807741,
    0.808405,
    0.809069,
    0.809731,
    0.810393,
    0.811053,
    0.811713,
    0.812372,
    0.813029,
    0.813686,
    0.814342,
    0.814997,
    0.815652,
    0.816305,
    0.816957,
    0.817608,
    0.818259,
    0.818908,
    0.819557,
    0.820204,
    0.820851,
    0.821497,
    0.822142,
    0.822786,
    0.823429,
    0.824071,
    0.824712,
    0.825352,
    0.825991,
    0.826629,
    0.827266,
    0.827903,
    0.828538,
    0.829173,
    0.829806,
    0.830439,
    0.831070,
    0.831701,
    0.832331,
    0.832959,
    0.833587,
    0.834214,
    0.834840,
    0.835465,
    0.836089,
    0.836711,
    0.837333,
    0.837954,
    0.838575,
    0.839194,
    0.839812,
    0.840429,
    0.841045,
    0.841660,
    0.842274,
    0.842888,
    0.843500,
    0.844111,
    0.844721,
    0.845331,
    0.845939,
    0.846546,
    0.847153,
    0.847758,
    0.848362,
    0.848966,
    0.849568,
    0.850170,
    0.850770,
    0.851369,
    0.851968,
    0.852565,
    0.853161,
    0.853757,
    0.854351,
    0.854945,
    0.855537,
    0.856128,
    0.856719,
    0.857308,
    0.857896,
    0.858484,
    0.859070,
    0.859655,
    0.860239,
    0.860823,
    0.861405,
    0.861986,
    0.862566,
    0.863145,
    0.863724,
    0.864301,
    0.864877,
    0.865452,
    0.866026,
    0.866599,
    0.867171,
    0.867742,
    0.868312,
    0.868880,
    0.869448,
    0.870015,
    0.870581,
    0.871146,
    0.871709,
    0.872272,
    0.872833,
    0.873394,
    0.873954,
    0.874512,
    0.875069,
    0.875626,
    0.876181,
    0.876735,
    0.877289,
    0.877841,
    0.878392,
    0.878942,
    0.879491,
    0.880039,
    0.880586,
    0.881132,
    0.881677,
    0.882220,
    0.882763,
    0.883305,
    0.883845,
    0.884385,
    0.884923,
    0.885461,
    0.885997,
    0.886532,
    0.887067,
    0.887600,
    0.888132,
    0.888663,
    0.889193,
    0.889722,
    0.890250,
    0.890777,
    0.891302,
    0.891827,
    0.892351,
    0.892873,
    0.893395,
    0.893915,
    0.894435,
    0.894953,
    0.895470,
    0.895986,
    0.896502,
    0.897016,
    0.897529,
    0.898041,
    0.898551,
    0.899061,
    0.899570,
    0.900078,
    0.900584,
    0.901090,
    0.901594,
    0.902098,
    0.902600,
    0.903102,
    0.903602,
    0.904101,
    0.904599,
    0.905096,
    0.905592,
    0.906087,
    0.906581,
    0.907074,
    0.907566,
    0.908057,
    0.908547,
    0.909035,
    0.909523,
    0.910009,
    0.910495,
    0.910979,
    0.911463,
    0.911945,
    0.912427,
    0.912907,
    0.913386,
    0.913864,
    0.914342,
    0.914818,
    0.915293,
    0.915767,
    0.916240,
    0.916712,
    0.917183,
    0.917653,
    0.918122,
    0.918590,
    0.919057,
    0.919523,
    0.919987,
    0.920451,
    0.920914,
    0.921376,
    0.921837,
    0.922297,
    0.922755,
    0.923213,
    0.923670,
    0.924126,
    0.924581,
    0.925034,
    0.925487,
    0.925939,
    0.926390,
    0.926840,
    0.927288,
    0.927736,
    0.928183,
    0.928629,
    0.929074,
    0.929518,
    0.929961,
    0.930403,
    0.930844,
    0.931285,
    0.931724,
    0.932162,
    0.932599,
    0.933036,
    0.933471,
    0.933906,
    0.934339,
    0.934772,
    0.935204,
    0.935635,
    0.936064,
    0.936493,
    0.936921,
    0.937349,
    0.937775,
    0.938200,
    0.938625,
    0.939048,
    0.939471,
    0.939893,
    0.940314,
    0.940734,
    0.941153,
    0.941571,
    0.941989,
    0.942405,
    0.942821,
    0.943236,
    0.943650,
    0.944063,
    0.944476,
    0.944888,
    0.945298,
    0.945708,
    0.946117,
    0.946526,
    0.946933,
    0.947340,
    0.947746,
    0.948151,
    0.948556,
    0.948959,
    0.949362,
    0.949764,
    0.950166,
    0.950566,
    0.950966,
    0.951365,
    0.951764,
    0.952162,
    0.952559,
    0.952955,
    0.953350,
    0.953745,
    0.954139,
    0.954533,
    0.954926,
    0.955318,
    0.955709,
    0.956100,
    0.956490,
    0.956880,
    0.957269,
    0.957657,
    0.958045,
    0.958432,
    0.958818,
    0.959204,
    0.959589,
    0.959974,
    0.960358,
    0.960742,
    0.961125,
    0.961507,
    0.961889,
    0.962270,
    0.962651,
    0.963031,
    0.963411,
    0.963790,
    0.964169,
    0.964548,
    0.964925,
    0.965303,
    0.965680,
    0.966056,
    0.966432,
    0.966808,
    0.967183,
    0.967558,
    0.967932,
    0.968306,
    0.968679,
    0.969053,
    0.969425,
    0.969798,
    0.970170,
    0.970542,
    0.970913,
    0.971284,
    0.971655,
    0.972026,
    0.972396,
    0.972766,
    0.973136,
    0.973505,
    0.973874,
    0.974243,
    0.974612,
    0.974981,
    0.975349,
    0.975717,
    0.976085,
    0.976453,
    0.976821,
    0.977188,
    0.977556,
    0.977923,
    0.978290,
    0.978657,
    0.979025,
    0.979391,
    0.979758,
    0.980125,
    0.980492,
    0.980859,
    0.981226,
    0.981592,
    0.981959,
    0.982326,
    0.982693,
    0.983060,
    0.983427,
    0.983794,
    0.984161,
    0.984528,
    0.984896,
    0.985263,
    0.985631,
    0.985999,
    0.986367,
    0.986735,
    0.987104,
    0.987472,
    0.987841,
    0.988210,
    0.988580,
    0.988949,
    0.989319,
    0.989690,
    0.990060,
    0.990431,
    0.990803,
    0.991174,
    0.991546,
    0.991919,
    0.992292,
    0.992665,
    0.993039,
    0.993413,
    0.993787,
    0.994163,
    0.994538,
    0.994914,
    0.995291,
    0.995669,
    0.996046,
    0.996425,
    0.996804,
    0.997184,
    0.997564,
    0.997945,
    0.998327,
    0.998709,
    0.999092,
    0.999476,
    0.999861,
    1.000246,
    1.000632,
    1.001019,
    1.001407,
    1.001795,
    1.002185,
    1.002575,
};

AnalogValue advanced_key_normalize(AdvancedKey* key, AnalogValue value)
{
    AnalogValue x;
    x = (key->upper_bound - value) / (key->upper_bound - key->lower_bound);
    uint16_t index = x / 0.001;
    if (index < 1000)
        return table[index];
    if (index > 5000)
        return 0;
    return 1.002575;
}


void keyboard_hid_send(uint8_t *report, uint16_t len)
{
#ifdef CONFIG_CHERRYUSB
hid_keyboard_send(report);
#else
USBHS_Endp_DataUp(HID_KEYBOARD_INT_EP,report,len,DEF_UEP_CPY_LOAD);
#endif
}
void launcherpage_open_menu();
void key_update1(Key* key, bool state)
{
    if ((!(key->state)) && state)
    {
        if (g_keyboard_send_report_enable)
        {   
            if (g_current_layer == 1)
            {
                launcherpage_open_menu();
                //fezui_frame_navigate(&g_mainframe, &launcherpage);
            }
            else
            {
                g_current_layer = g_current_layer == 2 ? 0 : 2;
            }
        }
        else if (key->key_cb[KEY_EVENT_DOWN])
        {
            key->key_cb[KEY_EVENT_DOWN](key);
        }
    }
    if ((key->state) && (!state))
    {
        if (key->key_cb[KEY_EVENT_UP])
            key->key_cb[KEY_EVENT_UP](key);
    }
    key->state = state;
}

void key_update2(Key* key, bool state)
{
    if ((!(key->state)) && state)
    {
        if (g_keyboard_send_report_enable)
        {
            if (g_current_layer == 2)
            {
                launcherpage_open_menu();
                //fezui_frame_navigate(&g_mainframe, &launcherpage);
            }
            else
            {
                g_current_layer = g_current_layer == 1 ? 0 : 1;
            }
        }
        else if (key->key_cb[KEY_EVENT_DOWN])
        {
            key->key_cb[KEY_EVENT_DOWN](key);
        }
    }
    if ((key->state) && (!state))
    {
        if (key->key_cb[KEY_EVENT_UP])
            key->key_cb[KEY_EVENT_UP](key);
    }
    key->state = state;
}

#define KEY_SCAN(key, pin_state) {\
bool state = (*(key)).state;\
key_update((key), (pin_state));\
if ((*(key)).state && !state) { keyboard_event_handler(MK_EVENT((*(key)).id,KEY_EVENT_DOWN)); }\
}

void keyboard_scan()
{
    KEY_SCAN(&KEY_FN_K1, FN_K1_READ);
    KEY_SCAN(&KEY_FN_K2, FN_K2_READ);
    KEY_SCAN(&KEY_FN_K3, FN_K3_READ);
    KEY_SCAN(&KEY_FN_K4, FN_K4_READ);
    KEY_SCAN(&KEY_FN_K5, FN_K5_READ);
    KEY_SCAN(&KEY_FN_K6, FN_K6_READ);
    KEY_SCAN(&KEY_KNOB, KNOB_READ);
    if (g_keyboard_knob_flag)
    {
        g_keyboard_knob_flag--;
    }
    if (!g_keyboard_knob_flag)
    {
        KEY_SCAN(&KEY_KNOB_CLOCKWISE, false);
        KEY_SCAN(&KEY_KNOB_ANTICLOCKWISE, false);
    }
}
void keyboard_system_reset()
{
    //__set_FAULTMASK(1);
    __disable_irq();
    NVIC_SystemReset();
}
void keyboard_post_process()
{
    
}
void keyboard_delay(uint32_t ms)
{
    Delay_Ms(ms);
}


void mouse_hid_send(uint8_t *report, uint16_t len)
{
#ifdef CONFIG_CHERRYUSB
    hid_mouse_send(report);
#else
    //static uint8_t mouse_write_buffer[64];
    //memcpy(mouse_write_buffer+1, report, 4);
    //mouse_write_buffer[0]=1;
    //mouse_write_buffer[1]=0;
    //mouse_write_buffer[2]=0;
    //mouse_write_buffer[3]=0;
    //mouse_write_buffer[4]=0;
    //USBHS_Endp_DataUp(HID_MOUSE_INT_EP,mouse_write_buffer,64,DEF_UEP_CPY_LOAD);
    USBHS_Endp_DataUp(HID_MOUSE_INT_EP,report,len,DEF_UEP_CPY_LOAD);
#endif
}

void keyboard_user_handler(uint8_t code)
{
    switch (code)
    {
    case USER_OPENMENU:
        void launcherpage_open_menu();
        if (g_mainframe.pages[g_mainframe.current_page_index] == &launcherpage)
        {
            launcherpage_open_menu();
            layer_reset(0);
            layer_reset(1);
            layer_reset(2);
        }
        break;
    default:
        break;
    }
}