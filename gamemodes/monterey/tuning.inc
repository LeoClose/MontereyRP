#define TUNING_MAX          8
#define TUNING_DOMET        5.0
#define TUNING_SVIJET       9000
#define TUNING_REDOVA       8
#define TUNING_TD           40
#define TUNING_DIJELOVA     32
#define TUNING_X            30.0
#define TUNING_Y            110.0
#define TUNING_SIRINA       200.0
#define TUNING_RED_H        14.0
#define TUNING_POPRAVAK     400
#define TUNING_POPRAVAK_MOTOR 600
#define TUNING_GUME         200
#define TUNING_BOJA         300
#define TUNING_PAINTJOB     2500
#define TUNING_NEON         2000
#define TUNING_CEKAJ_MS     350
#define TUNING_PONOVI_MS    120
#define TUNING_ULAZ_MS      1500
#define TUNING_TRUBA        1500
#define TUNING_ISKRE        4000
#define TUNING_ZATAMNJENJE  2500
#define TUNING_TRUBA_DOMET  40.0
#define TUNING_TRUBA_MS     1000
#define TUNING_ISKRE_STUPANJ 1
#define TUNING_ISKRE_MS     300
#define TUNING_ISKRE_SANSA  20
#define TUNING_ISKRE_PAUZA  1500
#define TUNING_ISKRE_RAZMAK 4000
#define TUNING_NATPIS       5000
#define TUNING_NATPIS_DUZ   6
#define TUNING_NATPIS_OBJEKT 19483
#define TUNING_NATPIS_FONT  340
#define TUNING_NATPIS_MAX   150
#define TUNING_ISKRE_TRAJE  900
#define TUNING_ISKRE_FX_Z   1.611
#define TUNING_ISKRE_KRECE  0.03
#define TUNING_ISKRE_OBJEKT 18702

enum { TK_POPRAVAK, TK_BOJA1, TK_BOJA2, TK_PAINTJOB, TK_MOTOR, TK_NEON, TK_OKLOP, TK_REZERVOAR, TK_TRUBA, TK_ISKRE, TK_ZATAMNJENJE, TK_NATPIS, TK_DIO }
enum { TTD_PODNASLOV, TTD_BROJAC, TTD_KUPI, TTD_NATRAG, TTD_NOVAC, TTD_POMOC, TTD_RED }
enum E_TUNING { bool:tgIma, tgDb, Float:tgX, Float:tgY, Float:tgZ, Float:tgA, STREAMER_TAG_3D_TEXT_LABEL:tgTekst, STREAMER_TAG_MAP_ICON:tgIkona }

static const gTuningDio[][] = { "Spojler", "Hauba", "Krov", "Pragovi", "Svjetla", "Nitro", "Auspuh", "Felge", "Stereo", "Hidraulika", "Prednji branik", "Straznji branik", "Desni ventil", "Lijevi ventil" };
static const gTuningDioCijena[] = { 1500, 1200, 1000, 1200, 400, 0, 800, 2000, 500, 3000, 1500, 1500, 600, 600 };
static const gTuningDioPogled[] = { 1, 4, 3, 2, 4, 1, 1, 2, 0, 2, 4, 1, 3, 3 };
static const gTuningMotorCijena[] = { 0, 5000, 10000, 20000 };
static const gTuningNeon[][] = { "Bez", "Crveni", "Plavi", "Zeleni", "Zuti", "Ruzicasti", "Bijeli" };
static const gTuningOklopCijena[] = { 0, 4000, 8000, 15000 };
static const gTuningOklopPosto[] = { 0, 20, 35, 50 };
static const gTuningRezervoarCijena[] = { 0, 3000, 6000 };
static const gTuningTruba[][] = { "Tvornicka", "Klasicna", "Zracna", "Duga zracna", "Zvono", "Zvonce" };
static const gTuningTrubaZvuk[] = { 0, 1147, 3200, 3201, 3401, 6401 };
static const Float:gTuningAuspuh[212][4] = {
    { 0.358, -2.349, -0.665, 1.0 },
    { 0.447, -2.480, -0.466, 1.0 },
    { 0.491, -2.674, -0.493, 2.0 },
    { 1.105, 0.974, 2.404, 2.0 },
    { 0.443, -2.767, -0.432, 1.0 },
    { 0.481, -2.828, -0.584, 1.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { 0.567, -3.692, -0.904, 1.0 },
    { -0.902, 1.946, 2.068, 1.0 },
    { 0.624, -3.785, -0.482, 2.0 },
    { 0.394, -2.379, -0.444, 1.0 },
    { 0.311, -2.519, -0.461, 2.0 },
    { 0.433, -3.348, -0.547, 1.0 },
    { 0.677, -2.529, -0.863, 1.0 },
    { 0.520, -3.256, -0.714, 1.0 },
    { 0.377, -2.409, -0.488, 2.0 },
    { 0.500, -3.820, -0.796, 1.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { 0.569, -2.636, -0.823, 1.0 },
    { 0.566, -2.929, -0.538, 1.0 },
    { 0.576, -2.625, -0.521, 1.0 },
    { 0.448, -3.081, -0.577, 1.0 },
    { 0.422, -2.531, -0.753, 1.0 },
    { -0.374, -2.292, -0.784, 1.0 },
    { 0.515, -1.646, -0.235, 1.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { 0.570, -2.613, -0.527, 1.0 },
    { 0.596, -3.975, -0.725, 1.0 },
    { 0.596, -3.061, -0.860, 1.0 },
    { 0.505, -2.527, -0.348, 2.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { 0.789, -5.556, -0.797, 1.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { 0.620, -4.098, -1.012, 1.0 },
    { -0.825, 0.662, -0.046, 2.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { 0.487, -2.533, -0.501, 1.0 },
    { 0.789, -5.263, -0.852, 1.0 },
    { 0.481, -2.694, -0.757, 1.0 },
    { 0.346, -2.559, -0.426, 2.0 },
    { 0.414, -2.626, -0.954, 1.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { 0.418, -3.267, -0.537, 1.0 },
    { 1.302, 2.022, -1.101, 1.0 },
    { -0.752, -1.025, -0.098, 1.0 },
    { 0.483, -2.708, -0.527, 1.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { 0.127, -0.820, -0.317, 1.0 },
    { 0.659, -3.947, -1.109, 1.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { 0.001, -2.564, -0.369, 2.0 },
    { -0.572, -2.536, 1.338, 1.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { 0.564, -4.098, -0.793, 1.0 },
    { -0.701, -1.075, -0.716, 1.0 },
    { 0.474, -1.366, -0.281, 1.0 },
    { 0.525, -2.877, -0.610, 1.0 },
    { 0.640, -2.529, -0.863, 1.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { 0.253, -1.139, -0.007, 2.0 },
    { 0.127, -0.820, -0.317, 1.0 },
    { 0.274, -0.567, -0.294, 2.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { 0.518, -2.896, -0.470, 1.0 },
    { 0.955, -2.930, -0.479, 1.0 },
    { 0.073, -0.876, 0.169, 1.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { -1.256, -2.120, 1.149, 1.0 },
    { 0.176, -0.756, 0.089, 1.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { 0.352, -2.245, 0.593, 1.0 },
    { 0.588, -2.825, -0.586, 2.0 },
    { 0.485, -2.582, -0.499, 1.0 },
    { -0.470, 2.505, -0.165, 1.0 },
    { 0.588, -2.892, -0.444, 1.0 },
    { 0.417, -2.658, -0.575, 1.0 },
    { 0.457, -2.833, -0.464, 1.0 },
    { 0.489, -2.401, -0.544, 1.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { 0.445, -2.600, -0.825, 1.0 },
    { -0.336, -2.824, -0.895, 1.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { 0.261, 1.176, 0.950, 2.0 },
    { -0.310, -3.027, 2.175, 1.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { 0.442, -2.713, -0.618, 1.0 },
    { 0.385, -3.219, -0.623, 1.0 },
    { 0.580, -2.991, -0.567, 1.0 },
    { 0.575, -2.608, -0.497, 1.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { 1.043, -1.344, -0.746, 2.0 },
    { 0.588, -2.259, -0.713, 1.0 },
    { 0.552, -2.150, -0.490, 1.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { -0.479, -3.089, -0.738, 1.0 },
    { 0.515, -3.383, -0.684, 1.0 },
    { 0.452, -2.011, -0.599, 1.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { 1.018, -1.196, -0.745, 2.0 },
    { 1.029, -1.352, -0.730, 2.0 },
    { 0.518, -2.896, -0.470, 1.0 },
    { 0.442, -2.713, -0.618, 1.0 },
    { 0.552, -2.461, -0.500, 2.0 },
    { 0.491, -2.961, -0.644, 1.0 },
    { 0.474, -3.632, -1.071, 1.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { -2.998, 0.051, -0.680, 1.0 },
    { -0.469, 1.925, -0.076, 1.0 },
    { 0.000, 0.955, -0.860, 1.0 },
    { 1.176, 0.579, 2.161, 2.0 },
    { -1.368, -1.024, 1.941, 1.0 },
    { 0.449, -3.016, -0.520, 1.0 },
    { 0.500, -2.876, -0.511, 1.0 },
    { -0.423, -2.848, -0.456, 1.0 },
    { 1.416, -5.809, 0.393, 1.0 },
    { 0.764, -2.803, -0.022, 1.0 },
    { 0.218, -1.223, 0.274, 1.0 },
    { 0.189, -1.166, 0.134, 2.0 },
    { -0.210, -1.073, -0.171, 2.0 },
    { 1.009, 0.811, 1.383, 1.0 },
    { 0.493, -2.824, -0.471, 1.0 },
    { 0.484, -2.301, -0.507, 1.0 },
    { 0.474, -2.366, -0.382, 1.0 },
    { -0.627, -2.673, -0.688, 1.0 },
    { -0.428, -2.666, -0.389, 1.0 },
    { 0.435, -1.120, -0.443, 1.0 },
    { -0.468, 0.554, 1.139, 1.0 },
    { 0.904, 2.350, 2.185, 1.0 },
    { 0.512, -2.433, -0.427, 2.0 },
    { 0.474, -2.922, -0.546, 2.0 },
    { 0.349, -2.804, -0.555, 2.0 },
    { 0.503, -2.969, -0.535, 2.0 },
    { 0.659, -7.830, -1.904, 1.0 },
    { 0.000, -0.075, 2.056, 1.0 },
    { -0.729, -1.609, 0.564, 1.0 },
    { -0.402, -2.734, -0.640, 1.0 },
    { 0.628, -2.185, -0.259, 2.0 },
    { 0.590, -2.917, -0.380, 1.0 },
    { -0.400, -2.712, -0.476, 1.0 },
    { 1.148, -0.979, -0.904, 1.0 },
    { 0.309, -2.265, -0.648, 1.0 },
    { 0.584, -2.814, -0.395, 1.0 },
    { 0.630, -2.626, -0.450, 1.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { 0.448, -2.470, -0.418, 1.0 },
    { -0.627, -2.911, -0.649, 1.0 },
    { 0.594, -3.235, -0.481, 1.0 },
    { 0.447, -3.032, -0.311, 1.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { 0.751, -2.741, -0.589, 1.0 },
    { 0.138, -2.266, -0.380, 2.0 },
    { 0.751, -2.795, 0.141, 1.0 },
    { 0.751, -2.741, 0.027, 1.0 },
    { -0.464, -2.551, -0.399, 2.0 },
    { 0.633, -2.369, -0.351, 2.0 },
    { -0.515, -2.391, -0.357, 2.0 },
    { -0.440, -2.794, -0.556, 2.0 },
    { -0.475, -2.352, -0.413, 2.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { -0.488, -2.115, -0.375, 2.0 },
    { 0.565, -2.943, -0.424, 1.0 },
    { 0.504, -2.737, -0.631, 1.0 },
    { 0.454, -1.722, -0.034, 1.0 },
    { 0.659, -7.830, -1.904, 1.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { -0.266, -0.778, -0.064, 1.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { -1.143, 1.080, 1.696, 2.0 },
    { 0.589, -1.161, -0.517, 1.0 },
    { 0.443, -2.718, -0.360, 1.0 },
    { 0.652, -3.095, -0.433, 1.0 },
    { -2.233, 0.301, 1.384, 1.0 },
    { 0.502, -4.814, -0.915, 1.0 },
    { -0.429, -2.850, -0.481, 1.0 },
    { -0.409, -2.770, -0.474, 1.0 },
    { 0.225, -1.012, 0.042, 1.0 },
    { 0.443, -3.389, -0.723, 1.0 },
    { -0.846, 0.796, 0.083, 1.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { -0.484, -3.146, -0.279, 1.0 },
    { 0.190, -1.268, -0.248, 2.0 },
    { 0.699, -2.781, -0.455, 2.0 },
    { -0.575, -4.021, -0.714, 1.0 },
    { -0.575, -2.344, -0.353, 2.0 },
    { 0.659, -7.830, -1.904, 1.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { 0.593, -2.665, -0.393, 1.0 },
    { 0.593, -2.665, -0.393, 1.0 },
    { 0.596, -2.665, -0.383, 1.0 },
    { 0.441, -2.672, -0.622, 1.0 },
    { 0.445, -2.744, -0.441, 1.0 },
    { -0.409, -3.133, 1.426, 1.0 },
    { 0.553, -2.524, -0.582, 2.0 },
    { 0.584, -2.651, -0.557, 2.0 },
    { 0.518, -2.896, -0.470, 1.0 },
    { -0.400, -2.712, -0.476, 1.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { -0.479, -3.089, -0.738, 1.0 },
    { 0.000, 0.000, 0.000, 0.0 },
    { 0.000, 0.000, 0.000, 0.0 }
};

static const Float:gTuningKrov[212][2] = {
    { -0.784, 0.805 },
    { -0.712, 0.741 },
    { -0.354, 0.726 },
    { 0.000, 2.492 },
    { -0.602, 0.903 },
    { -0.716, 0.725 },
    { 0.000, 1.933 },
    { 1.641, 1.657 },
    { 0.000, 2.118 },
    { -0.690, 0.790 },
    { -0.495, 0.875 },
    { -0.419, 0.675 },
    { 0.192, 0.655 },
    { -0.783, 1.113 },
    { 0.000, 2.300 },
    { -0.769, 0.588 },
    { 0.000, 1.693 },
    { -0.000, 4.079 },
    { -0.382, 0.992 },
    { -0.386, 0.656 },
    { -0.412, 1.072 },
    { -0.284, 0.675 },
    { -0.046, 0.773 },
    { 1.553, 2.452 },
    { 0.000, 0.928 },
    { -0.000, 2.945 },
    { -0.289, 0.817 },
    { -0.869, 1.668 },
    { 0.000, 1.568 },
    { -0.224, 0.582 },
    { 0.000, 0.000 },
    { 2.152, 2.193 },
    { 0.000, 1.108 },
    { 0.000, 1.732 },
    { 0.000, 0.807 },
    { 0.000, 2.483 },
    { -0.667, 0.823 },
    { -0.206, 2.051 },
    { 0.024, 0.923 },
    { 0.133, 0.654 },
    { -0.642, 1.206 },
    { -0.718, 0.425 },
    { -1.382, 0.880 },
    { 0.000, 1.941 },
    { 0.127, 1.650 },
    { -0.603, 0.801 },
    { 0.000, 0.000 },
    { -0.000, 1.819 },
    { -0.688, 0.517 },
    { 0.185, 1.914 },
    { 0.000, 1.532 },
    { -0.649, 0.550 },
    { 0.000, 2.945 },
    { 0.000, 0.000 },
    { 0.000, 0.000 },
    { 0.000, 1.732 },
    { 0.000, 2.311 },
    { -0.063, 1.328 },
    { -0.906, 0.707 },
    { -0.803, 1.113 },
    { -6.220, 1.917 },
    { 0.466, 0.748 },
    { 0.403, 0.489 },
    { 0.436, 0.545 },
    { 0.107, 0.269 },
    { -0.400, 0.489 },
    { -0.186, 0.833 },
    { 0.116, 0.802 },
    { 0.285, 0.501 },
    { -0.000, 1.819 },
    { 0.000, 1.203 },
    { 0.195, 0.540 },
    { 0.000, 0.000 },
    { 0.000, 0.000 },
    { -0.327, 0.799 },
    { -0.296, 0.690 },
    { -4.839, 1.429 },
    { -0.400, 0.684 },
    { 0.310, 0.856 },
    { -0.694, 0.945 },
    { -0.509, 0.667 },
    { -0.187, 0.324 },
    { -0.650, 0.910 },
    { 0.508, 0.996 },
    { 0.000, 0.000 },
    { -1.106, 1.191 },
    { -2.920, 2.259 },
    { -7.949, 2.731 },
    { -0.672, 2.651 },
    { -0.449, 1.054 },
    { -0.307, 1.054 },
    { -0.594, 0.685 },
    { -0.560, 0.840 },
    { 0.000, 0.000 },
    { -0.551, 0.718 },
    { -1.855, 1.232 },
    { -0.521, 0.814 },
    { -7.946, 2.731 },
    { -1.700, 2.093 },
    { 0.000, 1.890 },
    { 0.401, 0.852 },
    { -0.400, 0.489 },
    { -0.443, 0.748 },
    { -0.150, 0.765 },
    { 0.000, 0.960 },
    { -0.449, 1.054 },
    { -0.542, 0.551 },
    { -0.416, 0.773 },
    { 0.000, 1.830 },
    { -0.791, 0.607 },
    { -0.188, 0.374 },
    { -9.628, 2.395 },
    { 0.000, 1.267 },
    { -3.789, 0.972 },
    { 0.000, 2.252 },
    { 0.000, 2.111 },
    { 0.112, 0.834 },
    { -0.568, 0.818 },
    { 0.061, 0.683 },
    { -7.559, 2.597 },
    { -6.260, 2.134 },
    { -0.208, 0.547 },
    { -0.172, 0.597 },
    { -0.904, 1.016 },
    { -3.740, 1.532 },
    { -0.484, 1.539 },
    { -0.264, 0.625 },
    { 0.050, 0.842 },
    { -1.055, 1.093 },
    { -0.856, 0.876 },
    { -0.005, 1.463 },
    { 0.564, 1.143 },
    { -0.313, 2.268 },
    { 0.318, 0.707 },
    { -0.288, 0.596 },
    { 0.228, 0.768 },
    { 0.410, 0.570 },
    { -3.873, 2.250 },
    { -3.901, 2.091 },
    { -1.153, 1.109 },
    { -0.568, 0.674 },
    { 0.101, 0.607 },
    { -0.219, 0.805 },
    { 0.262, 0.846 },
    { 0.000, 1.560 },
    { -0.524, 0.752 },
    { -0.640, 0.815 },
    { -0.472, 0.863 },
    { -11.837, 2.764 },
    { 0.060, 0.691 },
    { -0.181, 0.677 },
    { -0.507, 0.872 },
    { 1.460, 1.659 },
    { -9.419, 3.689 },
    { 0.192, 1.006 },
    { 0.286, 0.563 },
    { 0.298, 1.630 },
    { -0.365, 1.731 },
    { -0.459, 0.834 },
    { -0.108, 0.692 },
    { -0.655, 0.796 },
    { -1.264, 0.796 },
    { -0.376, 0.754 },
    { -0.011, 2.361 },
    { 0.148, 0.274 },
    { -0.694, 0.659 },
    { -0.538, 0.814 },
    { 0.675, 0.624 },
    { -0.258, 0.738 },
    { -0.015, -0.157 },
    { -0.037, 1.602 },
    { 0.140, 0.368 },
    { -0.155, 0.748 },
    { 0.000, 1.757 },
    { 0.454, 1.415 },
    { 0.699, 0.920 },
    { 0.346, 0.867 },
    { -26.640, 17.332 },
    { 3.386, 1.329 },
    { -1.082, 1.208 },
    { -0.191, 1.021 },
    { -0.518, 0.518 },
    { -1.374, 1.585 },
    { -0.333, 1.672 },
    { 0.000, 1.386 },
    { -0.096, 0.979 },
    { 0.452, 0.991 },
    { -0.327, 0.714 },
    { 0.686, 3.672 },
    { -0.645, 1.038 },
    { 0.000, 2.240 },
    { 0.000, 2.483 },
    { -24.180, 9.178 },
    { -6.128, 1.918 },
    { 0.022, 0.304 },
    { -0.053, 4.049 },
    { -0.384, 1.000 },
    { -0.384, 1.000 },
    { -0.340, 1.055 },
    { 0.000, 2.465 },
    { -0.069, 0.749 },
    { -0.433, 1.581 },
    { -0.494, 0.659 },
    { -0.737, 0.642 },
    { -0.186, 0.833 },
    { 0.262, 0.846 },
    { -1.404, 1.102 },
    { 0.000, -0.001 },
    { 0.000, 0.676 },
    { -1.700, 2.093 },
    { -0.087, 0.793 },
    { -0.113, 1.104 }
};
static const gTuningFont[][] = { "Arial", "Impact", "Verdana", "Comic Sans MS", "Courier New", "Georgia", "Tahoma", "Times New Roman" };
static const gTuningNatpisBoja[][] = { "Bijela", "Crvena", "Plava", "Zelena", "Zuta", "Ruzicasta", "Crna", "Zlatna" };
static const gTuningNatpisArgb[] = { 0xFFFFFFFF, 0xFFE02020, 0xFF2080FF, 0xFF20D040, 0xFFFFE020, 0xFFFF69B4, 0xFF000000, 0xFFFFC020 };

static const Float:gTuningPogled[][3] = { { -4.5, 5.0, 1.6 }, { 0.0, -6.5, 1.6 }, { -4.5, 0.0, 0.5 }, { 0.0, -3.5, 4.5 }, { 0.0, 6.5, 1.4 }, { 0.0, -0.3, 5.5 } };

static gTuning[TUNING_MAX][E_TUNING];
static gTunMod[MAX_VEHICLES][14], gTunPaint[MAX_VEHICLES], gTunNeon[MAX_VEHICLES], gTunMotor[MAX_VEHICLES], gTunBoja[MAX_VEHICLES][2], gTunNeonObj[MAX_VEHICLES][2];
static gTunOklop[MAX_VEHICLES], gTunRezervoar[MAX_VEHICLES], gTunTruba[MAX_VEHICLES], gTunIskre[MAX_VEHICLES], Float:gTunZdravlje[MAX_VEHICLES], gTunIskreMs[MAX_VEHICLES], gTunTrubaMs[MAX_VEHICLES], gTunZatamnjeno[MAX_VEHICLES];
static bool:gTunSkriven[MAX_PLAYERS], gTunTrubaStaro[MAX_PLAYERS], gTunTrubaSlusa[MAX_PLAYERS];
static gTunNatpis[MAX_VEHICLES][TUNING_NATPIS_DUZ + 1], gTunNatpisFont[MAX_VEHICLES], gTunNatpisBoja[MAX_VEHICLES], gTunNatpisObj[MAX_VEHICLES];
static gTunNTekst[MAX_PLAYERS][TUNING_NATPIS_DUZ + 1], gTunNFont[MAX_PLAYERS], gTunNBoja[MAX_PLAYERS], bool:gTunUnos[MAX_PLAYERS];
static gTunU[MAX_PLAYERS], gTunV[MAX_PLAYERS], gTunKat[MAX_PLAYERS], gTunIdx[MAX_PLAYERS], gTunPoc[MAX_PLAYERS], gTunBroj[MAX_PLAYERS];
static gTunDijelovi[MAX_PLAYERS][TUNING_DIJELOVA], gTunKategorije[MAX_PLAYERS][TK_DIO + 14], gTunKatBroj[MAX_PLAYERS];
static PlayerText:gTunTd[MAX_PLAYERS][TUNING_TD], gTunTdBroj[MAX_PLAYERS], PlayerText:gTun[MAX_PLAYERS][TTD_RED + TUNING_REDOVA * 3];
static gApTuning[MAX_PLAYERS], gTunSmjer[MAX_PLAYERS], gTunPonovi[MAX_PLAYERS], gTunMotorRadi[MAX_PLAYERS], gTunOd[MAX_PLAYERS], gTunStrana[MAX_PLAYERS], bool:gTunMis[MAX_PLAYERS];

InitTuning()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS tuning_garages (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS vehicle_tuning (vehicle INTEGER PRIMARY KEY, mods TEXT NOT NULL DEFAULT '', paintjob INTEGER NOT NULL DEFAULT 3, neon INTEGER NOT NULL DEFAULT 0, motor INTEGER NOT NULL DEFAULT 0, oklop INTEGER NOT NULL DEFAULT 0, rezervoar INTEGER NOT NULL DEFAULT 0, truba INTEGER NOT NULL DEFAULT 0, plamen INTEGER NOT NULL DEFAULT 0, zatamnjeno INTEGER NOT NULL DEFAULT 0)"));
    new DBResult:stupac = DB_ExecuteQuery(gDB, "SELECT oklop FROM vehicle_tuning LIMIT 1");
    if (stupac) DB_FreeResultSet(stupac);
    else
    {
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "ALTER TABLE vehicle_tuning ADD COLUMN oklop INTEGER NOT NULL DEFAULT 0"));
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "ALTER TABLE vehicle_tuning ADD COLUMN rezervoar INTEGER NOT NULL DEFAULT 0"));
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "ALTER TABLE vehicle_tuning ADD COLUMN truba INTEGER NOT NULL DEFAULT 0"));
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "ALTER TABLE vehicle_tuning ADD COLUMN plamen INTEGER NOT NULL DEFAULT 0"));
    }
    stupac = DB_ExecuteQuery(gDB, "SELECT zatamnjeno FROM vehicle_tuning LIMIT 1");
    if (stupac) DB_FreeResultSet(stupac);
    else DB_FreeResultSet(DB_ExecuteQuery(gDB, "ALTER TABLE vehicle_tuning ADD COLUMN zatamnjeno INTEGER NOT NULL DEFAULT 0"));
    stupac = DB_ExecuteQuery(gDB, "SELECT natpis FROM vehicle_tuning LIMIT 1");
    if (stupac) DB_FreeResultSet(stupac);
    else
    {
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "ALTER TABLE vehicle_tuning ADD COLUMN natpis TEXT NOT NULL DEFAULT ''"));
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "ALTER TABLE vehicle_tuning ADD COLUMN natpis_font INTEGER NOT NULL DEFAULT 0"));
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "ALTER TABLE vehicle_tuning ADD COLUMN natpis_boja INTEGER NOT NULL DEFAULT 0"));
    }
    SetTimer("TuningImena", 1000, true);
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a FROM tuning_garages ORDER BY id LIMIT %d", TUNING_MAX);
    for (new g = 0, n = DB_GetRowCount(res); g < n; g++, DB_SelectNextRow(res))
    {
        gTuning[g][tgDb] = DB_GetFieldInt(res, 0);
        gTuning[g][tgX] = DB_GetFieldFloat(res, 1);
        gTuning[g][tgY] = DB_GetFieldFloat(res, 2);
        gTuning[g][tgZ] = DB_GetFieldFloat(res, 3);
        gTuning[g][tgA] = DB_GetFieldFloat(res, 4);
        TuningStvori(g);
    }
    DB_FreeResultSet(res);
    for (new i = 0; i < MAX_PLAYERS; i++) gTunU[i] = -1;
}

static TuningStvori(g)
{
    gTuning[g][tgIma] = true;
    gTuning[g][tgTekst] = CreateDynamic3DTextLabel("Tuning\n[Y]", COLOR_ORG, gTuning[g][tgX], gTuning[g][tgY], gTuning[g][tgZ] + 0.8, 25.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0);
    gTuning[g][tgIkona] = CreateDynamicMapIcon(gTuning[g][tgX], gTuning[g][tgY], gTuning[g][tgZ], 27, 0, 0, 0);
}

TuningMotor(v)
{
    return v > 0 && v < MAX_VEHICLES ? gTunMotor[v] : 0;
}

static bool:TuningProzoriZatvoreni(v)
{
    new fl, fr, rl, rr;
    GetVehicleParamsCarWindows(v, fl, fr, rl, rr);
    return fl != 0 && fr != 0 && rl != 0 && rr != 0;
}

forward TuningImena();
public TuningImena()
{
    for (new p = 0; p < MAX_PLAYERS; p++)
    {
        if (!IsPlayerConnected(p)) continue;
        new v = GetPlayerVehicleID(p), bool:skriven = v && gTunZatamnjeno[v] && TuningProzoriZatvoreni(v);
        if (!skriven && !gTunSkriven[p]) continue;
        gTunSkriven[p] = skriven;
        for (new i = 0; i < MAX_PLAYERS; i++)
            if (i != p && IsPlayerConnected(i)) ShowPlayerNameTagForPlayer(i, p, !skriven || IsPlayerInVehicle(i, v));
    }
}

TuningRezervoar(v)
{
    return v > 0 && v < MAX_VEHICLES ? gTunRezervoar[v] : 0;
}

forward TuningIskreKraj(objekt);
public TuningIskreKraj(objekt)
{
    DestroyObject(objekt);
}

static TuningIskre(v)
{
    new m = GetVehicleModel(v) - 400;
    if (m < 0 || m >= sizeof gTuningAuspuh) return;
    new broj = floatround(gTuningAuspuh[m][3]);
    for (new i = 0; i < broj; i++)
    {
        new objekt = CreateObject(TUNING_ISKRE_OBJEKT, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
        AttachObjectToVehicle(objekt, v, i ? -gTuningAuspuh[m][0] : gTuningAuspuh[m][0], gTuningAuspuh[m][1], gTuningAuspuh[m][2] - TUNING_ISKRE_FX_Z, 0.0, 0.0, 0.0);
        SetTimerEx("TuningIskreKraj", TUNING_ISKRE_TRAJE, false, "i", objekt);
    }
}

TuningVozac(v)
{
    if (gTunOklop[v])
    {
        new Float:h;
        GetVehicleHealth(v, h);
        if (gTunZdravlje[v] > 0.0 && h > 0.0 && h < gTunZdravlje[v])
        {
            h = gTunZdravlje[v] - (gTunZdravlje[v] - h) * float(100 - gTuningOklopPosto[gTunOklop[v]]) / 100.0;
            SetVehicleHealth(v, h);
        }
        gTunZdravlje[v] = h;
    }
    new sad = GetTickCount();
    new Float:vx, Float:vy, Float:vz;
    GetVehicleVelocity(v, vx, vy, vz);
    if (gTunMotor[v] >= TUNING_ISKRE_STUPANJ && sad - gTunIskreMs[v] >= 0 && VectorSize(vx, vy, 0.0) >= TUNING_ISKRE_KRECE)
    {
        if (random(100) < TUNING_ISKRE_SANSA)
        {
            TuningIskre(v);
            gTunIskreMs[v] = sad + TUNING_ISKRE_PAUZA + random(TUNING_ISKRE_RAZMAK);
        }
        else gTunIskreMs[v] = sad + TUNING_ISKRE_MS;
    }
}

TuningTrubaSync(playerid, v)
{
    new tiha = gTuningTrubaZvuk[gTunTruba[v]] && !GetVehicleComponentInSlot(v, CARMODTYPE_HYDRAULICS) ? 1 : 0;
    if (GetPVarInt(playerid, "truba_tiha") != tiha)
    {
        SetPVarInt(playerid, "truba_tiha", tiha);
        SetPVarInt(playerid, "truba", 0);
    }
    new pritisnuta = GetPVarInt(playerid, "truba");
    if (pritisnuta && !gTunTrubaStaro[playerid])
    {
        new KEY:tipke, gore, strana;
        GetPlayerKeys(playerid, tipke, gore, strana);
        OnPlayerKeyStateChange(playerid, tipke | KEY_CROUCH, tipke & ~KEY_CROUCH);
    }
    else if (!pritisnuta && gTunTrubaStaro[playerid]) TuningTrubaStani(playerid);
    gTunTrubaStaro[playerid] = pritisnuta;
}

TuningTrubi(playerid)
{
    new v = GetPlayerVehicleID(playerid), zvuk = v ? gTuningTrubaZvuk[gTunTruba[v]] : 0, Float:x, Float:y, Float:z;
    if (!zvuk || GetVehicleComponentInSlot(v, CARMODTYPE_HYDRAULICS) || GetTickCount() - gTunTrubaMs[v] < TUNING_TRUBA_MS) return;
    gTunTrubaMs[v] = GetTickCount();
    GetVehiclePos(v, x, y, z);
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (!IsPlayerConnected(i) || GetPlayerVirtualWorld(i) != GetVehicleVirtualWorld(v) || !IsPlayerInRangeOfPoint(i, TUNING_TRUBA_DOMET, x, y, z)) continue;
        PlayerPlaySound(i, zvuk, x, y, z);
        gTunTrubaSlusa[i] = playerid + 1;
    }
}

static TuningTrubaStani(playerid)
{
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (gTunTrubaSlusa[i] != playerid + 1) continue;
        gTunTrubaSlusa[i] = 0;
        if (IsPlayerConnected(i)) PlayerPlaySound(i, 0, 0.0, 0.0, 0.0);
    }
}

static TuningNeon(v, boja)
{
    for (new i = 0; i < 2; i++)
    {
        if (gTunNeonObj[v][i]) DestroyObject(gTunNeonObj[v][i]);
        gTunNeonObj[v][i] = 0;
        if (!boja) continue;
        gTunNeonObj[v][i] = CreateObject(18646 + boja, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
        AttachObjectToVehicle(gTunNeonObj[v][i], v, i ? 0.9 : -0.9, 0.0, -0.55, 0.0, 0.0, 0.0);
    }
}

static bool:TuningNatpisMoze(model)
{
    return model >= 400 && model <= 611 && gTuningKrov[model - 400][1] > 0.0 && !BezKrova(model);
}

static TuningNatpis(v, const tekst[], font, boja)
{
    if (gTunNatpisObj[v]) DestroyObject(gTunNatpisObj[v]);
    gTunNatpisObj[v] = 0;
    new model = GetVehicleModel(v), duz = strlen(tekst);
    if (!duz || !TuningNatpisMoze(model)) return;
    gTunNatpisObj[v] = CreateObject(TUNING_NATPIS_OBJEKT, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    SetObjectMaterialText(gTunNatpisObj[v], tekst, 0, OBJECT_MATERIAL_SIZE_512x256, gTuningFont[font], min(TUNING_NATPIS_MAX, TUNING_NATPIS_FONT / duz), true, gTuningNatpisArgb[boja], 0, OBJECT_MATERIAL_TEXT_ALIGN_CENTRE);
    AttachObjectToVehicle(gTunNatpisObj[v], v, 0.0, gTuningKrov[model - 400][0], gTuningKrov[model - 400][1], 0.0, -90.0, 0.0);
}

static TuningVrati(v)
{
    for (new s = 0; s < 14; s++)
    {
        new c = GetVehicleComponentInSlot(v, CARMODTYPE:s);
        if (c && c != gTunMod[v][s]) RemoveVehicleComponent(v, c);
        if (gTunMod[v][s] && c != gTunMod[v][s]) AddVehicleComponent(v, gTunMod[v][s]);
    }
    ChangeVehiclePaintjob(v, gTunPaint[v]);
    ChangeVehicleColours(v, gTunBoja[v][0], gTunBoja[v][1]);
    TuningNeon(v, gTunNeon[v]);
    TuningNatpis(v, gTunNatpis[v], gTunNatpisFont[v], gTunNatpisBoja[v]);
}

TuningPrimijeni(v)
{
    new db = VlasnikDb(v);
    if (!db) return 0;
    for (new s = 0; s < 14; s++) gTunMod[v][s] = 0;
    gTunPaint[v] = 3;
    gTunNeon[v] = 0;
    gTunMotor[v] = 0;
    gTunOklop[v] = 0;
    gTunRezervoar[v] = 0;
    gTunTruba[v] = 0;
    gTunIskre[v] = 0;
    gTunZatamnjeno[v] = 0;
    gTunZdravlje[v] = 0.0;
    gTunNatpis[v][0] = EOS;
    gTunNatpisFont[v] = 0;
    gTunNatpisBoja[v] = 0;
    new mods[160], DBResult:res = DB_ExecuteQuery(gDB, "SELECT p.c1, p.c2, IFNULL(t.mods, ''), IFNULL(t.paintjob, 3), IFNULL(t.neon, 0), IFNULL(t.motor, 0), IFNULL(t.oklop, 0), IFNULL(t.rezervoar, 0), IFNULL(t.truba, 0), IFNULL(t.plamen, 0), IFNULL(t.zatamnjeno, 0), IFNULL(t.natpis, ''), IFNULL(t.natpis_font, 0), IFNULL(t.natpis_boja, 0) FROM player_vehicles p LEFT JOIN vehicle_tuning t ON t.vehicle = p.id WHERE p.id = %d", db);
    if (!DB_GetRowCount(res))
    {
        DB_FreeResultSet(res);
        return 0;
    }
    gTunBoja[v][0] = DB_GetFieldInt(res, 0);
    gTunBoja[v][1] = DB_GetFieldInt(res, 1);
    DB_GetFieldString(res, 2, mods, sizeof mods);
    gTunPaint[v] = DB_GetFieldInt(res, 3);
    gTunNeon[v] = clamp(DB_GetFieldInt(res, 4), 0, sizeof gTuningNeon - 1);
    gTunMotor[v] = clamp(DB_GetFieldInt(res, 5), 0, sizeof gTuningMotorCijena - 1);
    gTunOklop[v] = clamp(DB_GetFieldInt(res, 6), 0, sizeof gTuningOklopCijena - 1);
    gTunRezervoar[v] = clamp(DB_GetFieldInt(res, 7), 0, sizeof gTuningRezervoarCijena - 1);
    gTunTruba[v] = clamp(DB_GetFieldInt(res, 8), 0, sizeof gTuningTruba - 1);
    gTunIskre[v] = clamp(DB_GetFieldInt(res, 9), 0, 1);
    gTunZatamnjeno[v] = clamp(DB_GetFieldInt(res, 10), 0, 1);
    DB_GetFieldString(res, 11, gTunNatpis[v], TUNING_NATPIS_DUZ + 1);
    gTunNatpisFont[v] = clamp(DB_GetFieldInt(res, 12), 0, sizeof gTuningFont - 1);
    gTunNatpisBoja[v] = clamp(DB_GetFieldInt(res, 13), 0, sizeof gTuningNatpisArgb - 1);
    DB_FreeResultSet(res);
    for (new i = 0, c = 0; ; i++)
    {
        if ('0' <= mods[i] <= '9')
        {
            c = c * 10 + mods[i] - '0';
            continue;
        }
        if (c >= 1000 && c <= 1193 && VehicleCanHaveComponent(GetVehicleModel(v), c)) gTunMod[v][_:GetVehicleComponentType(c)] = c;
        c = 0;
        if (!mods[i]) break;
    }
    TuningVrati(v);
    return 1;
}

TuningMakni(v)
{
    TuningNeon(v, 0);
    gTunNatpis[v][0] = EOS;
    TuningNatpis(v, "", 0, 0);
    gTunMotor[v] = 0;
    gTunOklop[v] = 0;
    gTunRezervoar[v] = 0;
    gTunTruba[v] = 0;
    gTunIskre[v] = 0;
    gTunZatamnjeno[v] = 0;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM vehicle_tuning WHERE vehicle = %d", VlasnikDb(v)));
}

static TuningSpremi(v)
{
    new mods[160], dio[8];
    for (new s = 0; s < 14; s++)
    {
        if (!gTunMod[v][s]) continue;
        format(dio, sizeof dio, "%d ", gTunMod[v][s]);
        strcat(mods, dio);
    }
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO vehicle_tuning (vehicle, mods, paintjob, neon, motor, oklop, rezervoar, truba, plamen, zatamnjeno, natpis, natpis_font, natpis_boja) VALUES (%d, '%q', %d, %d, %d, %d, %d, %d, %d, %d, '%q', %d, %d)", VlasnikDb(v), mods, gTunPaint[v], gTunNeon[v], gTunMotor[v], gTunOklop[v], gTunRezervoar[v], gTunTruba[v], gTunIskre[v], gTunZatamnjeno[v], gTunNatpis[v], gTunNatpisFont[v], gTunNatpisBoja[v]));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE player_vehicles SET c1 = %d, c2 = %d WHERE id = %d", gTunBoja[v][0], gTunBoja[v][1], VlasnikDb(v)));
}

static TuningPaintjoba(model)
{
    switch (model)
    {
        case 483: return 1;
        case 575: return 2;
        case 534, 535, 536, 558, 559, 560, 561, 562, 565, 567, 576: return 3;
    }
    return 0;
}

static TuningNitroCijena(c)
{
    return c == 1010 ? 5000 : c == 1008 ? 3000 : 1500;
}

static PlayerText:TuningKutija(playerid, Float:x, Float:y, Float:w, Float:h, boja)
{
    new PlayerText:td = CreatePlayerTextDraw(playerid, x, y, "LD_SPAC:white");
    PlayerTextDrawFont(playerid, td, TEXT_DRAW_FONT_SPRITE_DRAW);
    PlayerTextDrawTextSize(playerid, td, w, h);
    PlayerTextDrawColour(playerid, td, boja);
    gTunTd[playerid][gTunTdBroj[playerid]++] = td;
    return td;
}

static PlayerText:TuningTekst(playerid, Float:x, Float:y, const tekst[], boja, Float:sirina, Float:visina, TEXT_DRAW_FONT:font = TEXT_DRAW_FONT_1, TEXT_DRAW_ALIGN:poravnanje = TEXT_DRAW_ALIGN_LEFT)
{
    new PlayerText:td = CreatePlayerTextDraw(playerid, x, y, tekst);
    PlayerTextDrawFont(playerid, td, font);
    PlayerTextDrawLetterSize(playerid, td, sirina, visina);
    PlayerTextDrawColour(playerid, td, boja);
    PlayerTextDrawSetShadow(playerid, td, 0);
    PlayerTextDrawSetOutline(playerid, td, 0);
    PlayerTextDrawAlignment(playerid, td, poravnanje);
    gTunTd[playerid][gTunTdBroj[playerid]++] = td;
    return td;
}

static TuningKamera(playerid, pogled)
{
    new v = gTunV[playerid], Float:x, Float:y, Float:z, Float:a;
    GetVehiclePos(v, x, y, z);
    GetVehicleZAngle(v, a);
    new Float:ox = gTuningPogled[pogled][0], Float:oy = gTuningPogled[pogled][1];
    SetPlayerCameraPos(playerid, x + ox * floatcos(a, degrees) - oy * floatsin(a, degrees), y + ox * floatsin(a, degrees) + oy * floatcos(a, degrees), z + gTuningPogled[pogled][2]);
    SetPlayerCameraLookAt(playerid, x, y, z + 0.3);
}

static TuningKatIme(playerid, kat, ime[], size = sizeof (ime))
{
    switch (kat)
    {
        case TK_POPRAVAK: format(ime, size, T(playerid, "Popravak"));
        case TK_BOJA1: format(ime, size, T(playerid, "Primarna boja"));
        case TK_BOJA2: format(ime, size, T(playerid, "Sekundarna boja"));
        case TK_PAINTJOB: format(ime, size, T(playerid, "Paintjob"));
        case TK_MOTOR: format(ime, size, T(playerid, "Motor"));
        case TK_NEON: format(ime, size, T(playerid, "Neon"));
        case TK_OKLOP: format(ime, size, T(playerid, "Ojacana karoserija"));
        case TK_REZERVOAR: format(ime, size, T(playerid, "Spremnik goriva"));
        case TK_TRUBA: format(ime, size, T(playerid, "Truba"));
        case TK_ISKRE: format(ime, size, T(playerid, "Iskre"));
        case TK_ZATAMNJENJE: format(ime, size, T(playerid, "Zatamnjeni prozori"));
        case TK_NATPIS: format(ime, size, T(playerid, "Natpis na krovu"));
        default: format(ime, size, T(playerid, gTuningDio[kat - TK_DIO]));
    }
}

static TuningStavka(playerid, i, &vrijednost, &cijena, ime[], size = sizeof (ime))
{
    new kat = gTunKat[playerid], v = gTunV[playerid];
    if (kat == -1)
    {
        vrijednost = gTunKategorije[playerid][i];
        cijena = -1;
        TuningKatIme(playerid, vrijednost, ime, size);
        return;
    }
    switch (kat)
    {
        case TK_POPRAVAK:
        {
            vrijednost = i;
            cijena = i == 0 ? TUNING_POPRAVAK : i == 1 ? TUNING_POPRAVAK_MOTOR : TUNING_GUME;
            format(ime, size, T(playerid, i == 0 ? ("Popravi karoseriju") : i == 1 ? ("Popravi motor") : ("Zamijeni gume")));
        }
        case TK_NATPIS:
        {
            vrijednost = i;
            cijena = i == 3 ? TUNING_NATPIS : -1;
            switch (i)
            {
                case 0: format(ime, size, T(playerid, "Tekst: %s"), gTunNTekst[playerid][0] ? gTunNTekst[playerid] : ("-"));
                case 1: format(ime, size, T(playerid, "Font: %s"), gTuningFont[gTunNFont[playerid]]);
                case 2: format(ime, size, T(playerid, "Boja: %s"), T(playerid, gTuningNatpisBoja[gTunNBoja[playerid]]));
                case 3: format(ime, size, T(playerid, "Spremi natpis"));
                default: format(ime, size, T(playerid, "Ukloni natpis"));
            }
        }
        case TK_BOJA1, TK_BOJA2:
        {
            vrijednost = i;
            cijena = gTunBoja[v][kat - TK_BOJA1] == i ? 0 : TUNING_BOJA;
            format(ime, size, T(playerid, "Boja %d"), i);
        }
        case TK_PAINTJOB:
        {
            vrijednost = i ? i - 1 : 3;
            cijena = gTunPaint[v] == vrijednost || !i ? 0 : TUNING_PAINTJOB;
            if (i) format(ime, size, T(playerid, "Paintjob %d"), i);
            else format(ime, size, T(playerid, "Bez"));
        }
        case TK_MOTOR:
        {
            vrijednost = i;
            cijena = gTuningMotorCijena[i];
            if (i) format(ime, size, T(playerid, "Stupanj %d"), i);
            else format(ime, size, T(playerid, "Tvornicki"));
        }
        case TK_NEON:
        {
            vrijednost = i;
            cijena = i ? TUNING_NEON : 0;
            format(ime, size, T(playerid, gTuningNeon[i]));
        }
        case TK_OKLOP:
        {
            vrijednost = i;
            cijena = gTuningOklopCijena[i];
            if (i) format(ime, size, T(playerid, "Stupanj %d (-%d%% stete)"), i, gTuningOklopPosto[i]);
            else format(ime, size, T(playerid, "Tvornicki"));
        }
        case TK_REZERVOAR:
        {
            vrijednost = i;
            cijena = gTuningRezervoarCijena[i];
            format(ime, size, "%d L", floatround(VOZILO_TANK * (1.0 + 0.5 * float(i))));
        }
        case TK_TRUBA:
        {
            vrijednost = i;
            cijena = i ? TUNING_TRUBA : 0;
            format(ime, size, T(playerid, gTuningTruba[i]));
        }
        case TK_ISKRE:
        {
            vrijednost = i;
            cijena = i ? TUNING_ISKRE : 0;
            format(ime, size, T(playerid, i ? ("Ugradeni") : ("Bez")));
        }
        case TK_ZATAMNJENJE:
        {
            vrijednost = i;
            cijena = i ? TUNING_ZATAMNJENJE : 0;
            format(ime, size, T(playerid, i ? ("Zatamnjeno") : ("Bez")));
        }
        default:
        {
            vrijednost = i ? gTunDijelovi[playerid][i - 1] : 0;
            cijena = !i ? 0 : kat - TK_DIO == 5 ? TuningNitroCijena(vrijednost) : gTuningDioCijena[kat - TK_DIO];
            if (i) format(ime, size, "%s %d", T(playerid, gTuningDio[kat - TK_DIO]), i);
            else format(ime, size, T(playerid, "Tvornicki"));
        }
    }
}

static bool:TuningUgradeno(playerid, vrijednost)
{
    new kat = gTunKat[playerid], v = gTunV[playerid];
    switch (kat)
    {
        case -1, TK_POPRAVAK, TK_NATPIS: return false;
        case TK_BOJA1, TK_BOJA2: return gTunBoja[v][kat - TK_BOJA1] == vrijednost;
        case TK_PAINTJOB: return gTunPaint[v] == vrijednost;
        case TK_MOTOR: return gTunMotor[v] == vrijednost;
        case TK_NEON: return gTunNeon[v] == vrijednost;
        case TK_OKLOP: return gTunOklop[v] == vrijednost;
        case TK_REZERVOAR: return gTunRezervoar[v] == vrijednost;
        case TK_TRUBA: return gTunTruba[v] == vrijednost;
        case TK_ISKRE: return gTunIskre[v] == vrijednost;
        case TK_ZATAMNJENJE: return gTunZatamnjeno[v] == vrijednost;
    }
    return gTunMod[v][kat - TK_DIO] == vrijednost;
}

static TuningPrikazi(playerid)
{
    new ime[48], cijena, vrijednost, tekst[48];
    if (gTunKat[playerid] == -1) format(ime, sizeof ime, T(playerid, "KATEGORIJE"));
    else TuningKatIme(playerid, gTunKat[playerid], ime);
    PlayerTextDrawSetString(playerid, gTun[playerid][TTD_PODNASLOV], ime);
    format(tekst, sizeof tekst, "%d / %d", gTunIdx[playerid] + 1, gTunBroj[playerid]);
    PlayerTextDrawSetString(playerid, gTun[playerid][TTD_BROJAC], tekst);
    format(tekst, sizeof tekst, T(playerid, "Novac: ~g~$%d"), gNovac[playerid]);
    PlayerTextDrawSetString(playerid, gTun[playerid][TTD_NOVAC], tekst);
    for (new r = 0; r < TUNING_REDOVA; r++)
    {
        new i = gTunPoc[playerid] + r, PlayerText:pozadina = gTun[playerid][TTD_RED + r * 3], PlayerText:naziv = gTun[playerid][TTD_RED + r * 3 + 1], PlayerText:iznos = gTun[playerid][TTD_RED + r * 3 + 2];
        if (i >= gTunBroj[playerid])
        {
            PlayerTextDrawHide(playerid, pozadina);
            PlayerTextDrawHide(playerid, naziv);
            PlayerTextDrawHide(playerid, iznos);
            continue;
        }
        new bool:odabran = i == gTunIdx[playerid];
        TuningStavka(playerid, i, vrijednost, cijena, ime);
        if (cijena < 0) tekst = ">";
        else if (TuningUgradeno(playerid, vrijednost)) format(tekst, sizeof tekst, T(playerid, "Ugradeno"));
        else if (!cijena) tekst = "$0";
        else format(tekst, sizeof tekst, "$%d", cijena);
        PlayerTextDrawColour(playerid, pozadina, odabran ? 0xF0F0F0EE : 0x000000B4);
        PlayerTextDrawColour(playerid, naziv, odabran ? 0x000000FF : 0xFFFFFFFF);
        PlayerTextDrawColour(playerid, iznos, odabran ? 0x000000FF : 0xFFFFFFFF);
        PlayerTextDrawSetString(playerid, naziv, ime);
        PlayerTextDrawSetString(playerid, iznos, tekst);
        PlayerTextDrawShow(playerid, pozadina);
        PlayerTextDrawShow(playerid, naziv);
        PlayerTextDrawShow(playerid, iznos);
    }
    if (gTunKat[playerid] == -1) PlayerTextDrawHide(playerid, gTun[playerid][TTD_KUPI]);
    else PlayerTextDrawShow(playerid, gTun[playerid][TTD_KUPI]);
}

static TuningPregled(playerid)
{
    new v = gTunV[playerid], kat = gTunKat[playerid], vrijednost, cijena, ime[48];
    TuningVrati(v);
    if (kat == -1) return;
    TuningStavka(playerid, gTunIdx[playerid], vrijednost, cijena, ime);
    switch (kat)
    {
        case TK_BOJA1: ChangeVehicleColours(v, vrijednost, gTunBoja[v][1]);
        case TK_BOJA2: ChangeVehicleColours(v, gTunBoja[v][0], vrijednost);
        case TK_PAINTJOB: ChangeVehiclePaintjob(v, vrijednost);
        case TK_NEON: TuningNeon(v, vrijednost);
        case TK_TRUBA: if (gTuningTrubaZvuk[vrijednost]) PlayerPlaySound(playerid, gTuningTrubaZvuk[vrijednost], 0.0, 0.0, 0.0);
        case TK_ISKRE: if (vrijednost) TuningIskre(v);
        case TK_NATPIS: TuningNatpis(v, gTunNTekst[playerid], gTunNFont[playerid], gTunNBoja[playerid]);
        case TK_POPRAVAK, TK_MOTOR, TK_OKLOP, TK_REZERVOAR, TK_ZATAMNJENJE: {}
        default:
        {
            new stari = GetVehicleComponentInSlot(v, CARMODTYPE:(kat - TK_DIO));
            if (stari) RemoveVehicleComponent(v, stari);
            if (vrijednost) AddVehicleComponent(v, vrijednost);
        }
    }
}

static TuningOtvoriKat(playerid, kat)
{
    new v = gTunV[playerid], model = GetVehicleModel(v);
    gTunKat[playerid] = kat;
    gTunIdx[playerid] = 0;
    gTunPoc[playerid] = 0;
    switch (kat)
    {
        case -1: gTunBroj[playerid] = gTunKatBroj[playerid];
        case TK_POPRAVAK: gTunBroj[playerid] = 3;
        case TK_BOJA1, TK_BOJA2: gTunBroj[playerid] = 256;
        case TK_PAINTJOB: gTunBroj[playerid] = TuningPaintjoba(model) + 1;
        case TK_MOTOR: gTunBroj[playerid] = sizeof gTuningMotorCijena;
        case TK_NEON: gTunBroj[playerid] = sizeof gTuningNeon;
        case TK_OKLOP: gTunBroj[playerid] = sizeof gTuningOklopCijena;
        case TK_REZERVOAR: gTunBroj[playerid] = sizeof gTuningRezervoarCijena;
        case TK_TRUBA: gTunBroj[playerid] = sizeof gTuningTruba;
        case TK_ISKRE, TK_ZATAMNJENJE: gTunBroj[playerid] = 2;
        case TK_NATPIS:
        {
            gTunBroj[playerid] = 5;
            strcopy(gTunNTekst[playerid], gTunNatpis[v], TUNING_NATPIS_DUZ + 1);
            gTunNFont[playerid] = gTunNatpisFont[v];
            gTunNBoja[playerid] = gTunNatpisBoja[v];
        }
        default:
        {
            new n = 0;
            for (new c = 1000; c <= 1193 && n < TUNING_DIJELOVA; c++)
                if (_:GetVehicleComponentType(c) == kat - TK_DIO && VehicleCanHaveComponent(model, c)) gTunDijelovi[playerid][n++] = c;
            gTunBroj[playerid] = n + 1;
        }
    }
    if (kat >= 0)
    {
        new vrijednost, cijena, ime[48];
        for (new i = 0; i < gTunBroj[playerid]; i++)
        {
            TuningStavka(playerid, i, vrijednost, cijena, ime);
            if (!TuningUgradeno(playerid, vrijednost)) continue;
            gTunIdx[playerid] = i;
            gTunPoc[playerid] = i >= TUNING_REDOVA ? i - TUNING_REDOVA + 1 : 0;
            break;
        }
    }
    TuningKamera(playerid, kat == -1 || kat < TK_DIO ? (kat == TK_NEON ? 2 : kat == TK_MOTOR ? 4 : kat == TK_ISKRE ? 1 : kat == TK_NATPIS ? 5 : 0) : gTuningDioPogled[kat - TK_DIO]);
    TuningPregled(playerid);
    TuningPrikazi(playerid);
}

static TuningIzbornik(playerid)
{
    new v = gTunV[playerid], model = GetVehicleModel(v), n = 0;
    gTunKategorije[playerid][n++] = TK_POPRAVAK;
    gTunKategorije[playerid][n++] = TK_BOJA1;
    gTunKategorije[playerid][n++] = TK_BOJA2;
    if (TuningPaintjoba(model)) gTunKategorije[playerid][n++] = TK_PAINTJOB;
    gTunKategorije[playerid][n++] = TK_MOTOR;
    gTunKategorije[playerid][n++] = TK_NEON;
    gTunKategorije[playerid][n++] = TK_OKLOP;
    if (!BezMotora(model)) gTunKategorije[playerid][n++] = TK_REZERVOAR;
    gTunKategorije[playerid][n++] = TK_TRUBA;
    if (!BezKrova(model)) gTunKategorije[playerid][n++] = TK_ZATAMNJENJE;
    if (gPremium[playerid] && TuningNatpisMoze(model)) gTunKategorije[playerid][n++] = TK_NATPIS;
    for (new s = 0; s < 14; s++)
    {
        for (new c = 1000; c <= 1193; c++)
        {
            if (s == _:CARMODTYPE_NITRO || _:GetVehicleComponentType(c) != s || !VehicleCanHaveComponent(model, c)) continue;
            gTunKategorije[playerid][n++] = TK_DIO + s;
            break;
        }
    }
    gTunKatBroj[playerid] = n;
}

static TuningOtvori(playerid, g)
{
    new v = GetPlayerVehicleID(playerid);
    MobitelZatvori(playerid);
    gTunSmjer[playerid] = 0;
    gTunStrana[playerid] = 0;
    gTunU[playerid] = g;
    gTunV[playerid] = v;
    gTunTdBroj[playerid] = 0;
    SetVehiclePos(v, gTuning[g][tgX], gTuning[g][tgY], gTuning[g][tgZ]);
    SetVehicleZAngle(v, gTuning[g][tgA]);
    SetVehicleVelocity(v, 0.0, 0.0, 0.0);
    SetVehicleVirtualWorld(v, TUNING_SVIJET + playerid);
    SetPlayerVirtualWorld(playerid, TUNING_SVIJET + playerid);
    PutPlayerInVehicle(playerid, v, 0);
    new e, l, a, d, bn, bt, o;
    GetVehicleParamsEx(v, e, l, a, d, bn, bt, o);
    gTunMotorRadi[playerid] = e;
    SetVehicleParamsEx(v, 0, l, a, d, bn, bt, o);
    gTunOd[playerid] = GetTickCount();
    TuningKutija(playerid, TUNING_X, TUNING_Y, TUNING_SIRINA, 40.0, 0x1E5AA8F0);
    TuningTekst(playerid, TUNING_X + TUNING_SIRINA / 2.0, TUNING_Y + 6.0, "TUNING", 0xFFFFFFFF, 0.7, 2.8, TEXT_DRAW_FONT_3, TEXT_DRAW_ALIGN_CENTER);
    TuningKutija(playerid, TUNING_X, TUNING_Y + 40.0, TUNING_SIRINA, TUNING_RED_H, 0x000000F0);
    gTun[playerid][TTD_PODNASLOV] = TuningTekst(playerid, TUNING_X + 4.0, TUNING_Y + 42.0, "_", 0x5B8DBFFF, 0.2, 1.0, TEXT_DRAW_FONT_2);
    gTun[playerid][TTD_BROJAC] = TuningTekst(playerid, TUNING_X + TUNING_SIRINA - 4.0, TUNING_Y + 42.0, "_", 0xFFFFFFFF, 0.2, 1.0, TEXT_DRAW_FONT_2, TEXT_DRAW_ALIGN_RIGHT);
    for (new r = 0; r < TUNING_REDOVA; r++)
    {
        new Float:y = TUNING_Y + 40.0 + TUNING_RED_H * float(r + 1);
        gTun[playerid][TTD_RED + r * 3] = TuningKutija(playerid, TUNING_X, y, TUNING_SIRINA, TUNING_RED_H, 0x000000B4);
        new PlayerText:naziv = TuningTekst(playerid, TUNING_X + 5.0, y + 2.0, "_", 0xFFFFFFFF, 0.22, 1.05);
        PlayerTextDrawTextSize(playerid, naziv, TUNING_X + TUNING_SIRINA - 2.0, TUNING_RED_H - 2.0);
        PlayerTextDrawSetSelectable(playerid, naziv, true);
        gTun[playerid][TTD_RED + r * 3 + 1] = naziv;
        gTun[playerid][TTD_RED + r * 3 + 2] = TuningTekst(playerid, TUNING_X + TUNING_SIRINA - 5.0, y + 2.0, "_", 0xFFFFFFFF, 0.22, 1.05, TEXT_DRAW_FONT_1, TEXT_DRAW_ALIGN_RIGHT);
    }
    new Float:dno = TUNING_Y + 40.0 + TUNING_RED_H * float(TUNING_REDOVA + 1) + 4.0;
    TuningKutija(playerid, TUNING_X, dno, TUNING_SIRINA, 46.0, 0x000000D2);
    gTun[playerid][TTD_POMOC] = TuningTekst(playerid, TUNING_X + TUNING_SIRINA / 2.0, dno + 34.0, T(playerid, "W/S: odabir  Lijevo: natrag  Desno: odaberi  Y: mis"), 0x8AA0B6FF, 0.16, 0.8, TEXT_DRAW_FONT_1, TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawTextSize(playerid, gTun[playerid][TTD_POMOC], 10.0, TUNING_SIRINA);
    PlayerTextDrawSetSelectable(playerid, gTun[playerid][TTD_POMOC], true);
    gTun[playerid][TTD_NOVAC] = TuningTekst(playerid, TUNING_X + 5.0, dno + 3.0, "_", 0xFFFFFFFF, 0.2, 1.0);
    gTun[playerid][TTD_KUPI] = TuningTekst(playerid, TUNING_X + 150.0, dno + 18.0, T(playerid, "KUPI"), 0xFFFFFFFF, 0.24, 1.1, TEXT_DRAW_FONT_2, TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawUseBox(playerid, gTun[playerid][TTD_KUPI], true);
    PlayerTextDrawBoxColour(playerid, gTun[playerid][TTD_KUPI], 0x2E7D32FF);
    PlayerTextDrawTextSize(playerid, gTun[playerid][TTD_KUPI], 12.0, 90.0);
    PlayerTextDrawSetSelectable(playerid, gTun[playerid][TTD_KUPI], true);
    gTun[playerid][TTD_NATRAG] = TuningTekst(playerid, TUNING_X + 50.0, dno + 18.0, T(playerid, "NATRAG"), 0xFFFFFFFF, 0.24, 1.1, TEXT_DRAW_FONT_2, TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawUseBox(playerid, gTun[playerid][TTD_NATRAG], true);
    PlayerTextDrawBoxColour(playerid, gTun[playerid][TTD_NATRAG], 0x1E5AA8FF);
    PlayerTextDrawTextSize(playerid, gTun[playerid][TTD_NATRAG], 12.0, 90.0);
    PlayerTextDrawSetSelectable(playerid, gTun[playerid][TTD_NATRAG], true);
    for (new i = 0; i < gTunTdBroj[playerid]; i++) PlayerTextDrawShow(playerid, gTunTd[playerid][i]);
    TuningIzbornik(playerid);
    gTunMis[playerid] = false;
    TuningOtvoriKat(playerid, -1);
}

TuningZatvori(playerid, bool:uVozilo = true)
{
    new g = gTunU[playerid];
    if (g == -1) return;
    gTunU[playerid] = -1;
    new v = gTunV[playerid];
    for (new i = 0; i < gTunTdBroj[playerid]; i++) PlayerTextDrawDestroy(playerid, gTunTd[playerid][i]);
    gTunTdBroj[playerid] = 0;
    if (gTunMis[playerid]) CancelSelectTextDraw(playerid);
    gTunMis[playerid] = false;
    if (!GetVehicleModel(v)) return;
    TuningVrati(v);
    new e, l, a, d, bn, bt, o;
    GetVehicleParamsEx(v, e, l, a, d, bn, bt, o);
    SetVehicleParamsEx(v, gTunMotorRadi[playerid], l, a, d, bn, bt, o);
    SetVehicleVirtualWorld(v, 0);
    if (!IsPlayerConnected(playerid)) return;
    SetPlayerVirtualWorld(playerid, 0);
    if (uVozilo) PutPlayerInVehicle(playerid, v, 0);
    SetCameraBehindPlayer(playerid);
}

TuningStateChange(playerid, PLAYER_STATE:oldstate)
{
    if (oldstate == PLAYER_STATE_DRIVER) TuningTrubaStani(playerid);
    if (gTunU[playerid] == -1 || oldstate != PLAYER_STATE_DRIVER || GetTickCount() - gTunOd[playerid] < TUNING_ULAZ_MS) return;
    TuningZatvori(playerid, false);
}

static TuningTipkovnica(playerid)
{
    gTunMis[playerid] = false;
    CancelSelectTextDraw(playerid);
}

TuningMisIskljuci(playerid)
{
    gTunMis[playerid] = false;
}

TuningDisconnect(playerid)
{
    TuningTrubaStani(playerid);
    gTunTrubaSlusa[playerid] = 0;
    gTunUnos[playerid] = false;
    TuningZatvori(playerid);
    gTunU[playerid] = -1;
}

static TuningKupi(playerid)
{
    new v = gTunV[playerid], kat = gTunKat[playerid], vrijednost, cijena, ime[48];
    TuningStavka(playerid, gTunIdx[playerid], vrijednost, cijena, ime);
    if (TuningUgradeno(playerid, vrijednost)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "To je vec ugradeno."));
    new VEHICLE_PANEL_STATUS:paneli, VEHICLE_DOOR_STATUS:vrata, VEHICLE_LIGHT_STATUS:svjetla, VEHICLE_TYRE_STATUS:gume, Float:zdravlje;
    GetVehicleDamageStatus(v, paneli, vrata, svjetla, gume);
    GetVehicleHealth(v, zdravlje);
    if (kat == TK_POPRAVAK && ((vrijednost == 0 && !paneli && !vrata && !svjetla) || (vrijednost == 1 && zdravlje >= 1000.0) || (vrijednost == 2 && !gume)))
        return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nema sto popraviti."));
    if (cijena && !DajNovac(playerid, -cijena)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca."));
    switch (kat)
    {
        case TK_POPRAVAK:
        {
            switch (vrijednost)
            {
                case 0:
                {
                    RepairVehicle(v);
                    SetVehicleHealth(v, zdravlje);
                    UpdateVehicleDamageStatus(v, VEHICLE_PANEL_STATUS:0, VEHICLE_DOOR_STATUS:0, VEHICLE_LIGHT_STATUS:0, gume);
                }
                case 1:
                {
                    SetVehicleHealth(v, 1000.0);
                    gTunZdravlje[v] = 1000.0;
                }
                case 2: UpdateVehicleDamageStatus(v, paneli, vrata, svjetla, VEHICLE_TYRE_STATUS:0);
            }
        }
        case TK_BOJA1, TK_BOJA2: gTunBoja[v][kat - TK_BOJA1] = vrijednost;
        case TK_PAINTJOB: gTunPaint[v] = vrijednost;
        case TK_MOTOR: gTunMotor[v] = vrijednost;
        case TK_NEON: gTunNeon[v] = vrijednost;
        case TK_OKLOP: gTunOklop[v] = vrijednost;
        case TK_REZERVOAR: gTunRezervoar[v] = vrijednost;
        case TK_TRUBA: gTunTruba[v] = vrijednost;
        case TK_ISKRE: gTunIskre[v] = vrijednost;
        case TK_ZATAMNJENJE: gTunZatamnjeno[v] = vrijednost;
        default: gTunMod[v][kat - TK_DIO] = vrijednost;
    }
    if (kat != TK_POPRAVAK) TuningSpremi(v);
    PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
    TuningPregled(playerid);
    TuningPrikazi(playerid);
    return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Kupili ste: %s ($%d)."), ime, cijena);
}

static TuningPomakni(playerid, smjer)
{
    new n = gTunBroj[playerid];
    if (n < 1) return;
    gTunIdx[playerid] = (gTunIdx[playerid] + smjer + n) % n;
    if (gTunIdx[playerid] < gTunPoc[playerid]) gTunPoc[playerid] = gTunIdx[playerid];
    else if (gTunIdx[playerid] >= gTunPoc[playerid] + TUNING_REDOVA) gTunPoc[playerid] = gTunIdx[playerid] - TUNING_REDOVA + 1;
    if (gTunKat[playerid] != -1) TuningPregled(playerid);
    TuningPrikazi(playerid);
}

static TuningOdaberi(playerid)
{
    if (gTunKat[playerid] == -1) TuningOtvoriKat(playerid, gTunKategorije[playerid][gTunIdx[playerid]]);
    else if (gTunKat[playerid] == TK_NATPIS) TuningNatpisOdaberi(playerid);
    else TuningKupi(playerid);
}

static TuningNatpisPitaj(playerid, const greska[] = "")
{
    new body[192];
    gTunUnos[playerid] = true;
    format(body, sizeof body, T(playerid, "%sUnesite natpis (1-%d znakova, slova i brojevi):"), greska, TUNING_NATPIS_DUZ);
    return PrikaziDialog(playerid, DLG_TUNING_NATPIS, DIALOG_STYLE_INPUT, T(playerid, "Natpis na krovu"), body, T(playerid, "Potvrdi"), T(playerid, "Odustani"));
}

static TuningNatpisOdaberi(playerid)
{
    new v = gTunV[playerid];
    switch (gTunIdx[playerid])
    {
        case 0: return TuningNatpisPitaj(playerid);
        case 1: gTunNFont[playerid] = (gTunNFont[playerid] + 1) % sizeof gTuningFont;
        case 2: gTunNBoja[playerid] = (gTunNBoja[playerid] + 1) % sizeof gTuningNatpisArgb;
        case 3:
        {
            if (!gPremium[playerid]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ovo je samo za premium igrace."));
            if (!gTunNTekst[playerid][0]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Prvo upisite tekst."));
            if (!strcmp(gTunNTekst[playerid], gTunNatpis[v]) && gTunNFont[playerid] == gTunNatpisFont[v] && gTunNBoja[playerid] == gTunNatpisBoja[v]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "To je vec ugradeno."));
            if (!DajNovac(playerid, -TUNING_NATPIS)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca."));
            strcopy(gTunNatpis[v], gTunNTekst[playerid], TUNING_NATPIS_DUZ + 1);
            gTunNatpisFont[v] = gTunNFont[playerid];
            gTunNatpisBoja[v] = gTunNBoja[playerid];
            TuningSpremi(v);
            PlayerPlaySound(playerid, 1133, 0.0, 0.0, 0.0);
            SendClientMessage(playerid, COLOR_INFO, T(playerid, "Kupili ste: %s ($%d)."), gTunNatpis[v], TUNING_NATPIS);
        }
        default:
        {
            if (!gTunNatpis[v][0]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nema natpisa."));
            gTunNatpis[v][0] = EOS;
            gTunNTekst[playerid][0] = EOS;
            TuningSpremi(v);
            SendClientMessage(playerid, COLOR_INFO, T(playerid, "Natpis je uklonjen."));
        }
    }
    TuningPregled(playerid);
    TuningPrikazi(playerid);
    return 1;
}

DLG:tuning_natpis(playerid, response, const inputtext[])
{
    gTunUnos[playerid] = false;
    if (gTunU[playerid] == -1 || gTunKat[playerid] != TK_NATPIS || !response) return 1;
    new duz = strlen(inputtext);
    if (duz < 1 || duz > TUNING_NATPIS_DUZ) return TuningNatpisPitaj(playerid, T(playerid, "{FF6347}Natpis smije imati 1-6 znakova.\n\n"));
    for (new i = 0; i < duz; i++)
    {
        new c = inputtext[i];
        if (!(('a' <= c <= 'z') || ('A' <= c <= 'Z') || ('0' <= c <= '9') || c == ' ' || c == '-' || c == '!' || c == '.')) return TuningNatpisPitaj(playerid, T(playerid, "{FF6347}Dozvoljena su samo slova, brojevi, razmak i - ! .\n\n"));
    }
    strcopy(gTunNTekst[playerid], inputtext, TUNING_NATPIS_DUZ + 1);
    TuningPregled(playerid);
    TuningPrikazi(playerid);
    return 1;
}

static TuningNatrag(playerid)
{
    if (gTunKat[playerid] == -1)
    {
        TuningZatvori(playerid);
        return;
    }
    new kat = gTunKat[playerid];
    TuningOtvoriKat(playerid, -1);
    for (new i = 0; i < gTunKatBroj[playerid]; i++) if (gTunKategorije[playerid][i] == kat) gTunIdx[playerid] = i;
    gTunPoc[playerid] = gTunIdx[playerid] >= TUNING_REDOVA ? gTunIdx[playerid] - TUNING_REDOVA + 1 : 0;
    TuningPrikazi(playerid);
}

static bool:TuningProvjera(playerid)
{
    if (GetVehicleModel(gTunV[playerid]) && (GetPlayerVehicleID(playerid) == gTunV[playerid] || GetTickCount() - gTunOd[playerid] < TUNING_ULAZ_MS)) return true;
    TuningZatvori(playerid);
    return false;
}

TuningAktivan(playerid)
{
    return gTunU[playerid] != -1;
}

TuningKlik(playerid, PlayerText:td)
{
    if (gTunU[playerid] == -1) return 0;
    if (!TuningProvjera(playerid)) return 1;
    if (td == gTun[playerid][TTD_POMOC]) TuningTipkovnica(playerid);
    else if (td == gTun[playerid][TTD_NATRAG]) TuningNatrag(playerid);
    else if (td == gTun[playerid][TTD_KUPI])
    {
        if (gTunKat[playerid] != -1) TuningKupi(playerid);
    }
    else for (new r = 0; r < TUNING_REDOVA; r++)
    {
        if (td != gTun[playerid][TTD_RED + r * 3 + 1]) continue;
        new i = gTunPoc[playerid] + r;
        if (i >= gTunBroj[playerid]) break;
        gTunIdx[playerid] = i;
        if (gTunKat[playerid] == -1)
        {
            TuningOdaberi(playerid);
            break;
        }
        if (r == TUNING_REDOVA - 1 && i + 1 < gTunBroj[playerid]) gTunPoc[playerid]++;
        else if (!r && gTunPoc[playerid]) gTunPoc[playerid]--;
        if (gTunKat[playerid] != -1) TuningPregled(playerid);
        TuningPrikazi(playerid);
        break;
    }
    return 1;
}

TuningKey(playerid, KEY:newkeys, KEY:oldkeys)
{
    if (gTunU[playerid] == -1) return 0;
    if (gTunUnos[playerid]) return 1;
    if (!TuningProvjera(playerid)) return 1;
    if ((newkeys & KEY_YES) && !(oldkeys & KEY_YES))
    {
        if (gTunMis[playerid]) TuningTipkovnica(playerid);
        else
        {
            gTunMis[playerid] = true;
            SelectTextDraw(playerid, 0x5B8DBFFF);
        }
    }
    else if ((newkeys & KEY_ANALOG_RIGHT) && !(oldkeys & KEY_ANALOG_RIGHT)) TuningOdaberi(playerid);
    else if ((newkeys & KEY_ANALOG_LEFT) && !(oldkeys & KEY_ANALOG_LEFT)) TuningNatrag(playerid);
    return 1;
}

TuningUpdate(playerid)
{
    if (gTunU[playerid] == -1 || gTunUnos[playerid]) return;
    new KEY:tipke, gore, strana, smjer = 0, sad = GetTickCount();
    GetPlayerKeys(playerid, tipke, gore, strana);
    if (strana != gTunStrana[playerid])
    {
        gTunStrana[playerid] = strana;
        if (strana == KEY_RIGHT || strana == KEY_LEFT)
        {
            if (strana == KEY_RIGHT) TuningOdaberi(playerid);
            else TuningNatrag(playerid);
            return;
        }
    }
    if (gore < 0 || (tipke & KEY_SPRINT)) smjer = -1;
    else if (gore > 0 || (tipke & KEY_JUMP)) smjer = 1;
    if (!smjer)
    {
        gTunSmjer[playerid] = 0;
        return;
    }
    if (smjer == gTunSmjer[playerid] && sad < gTunPonovi[playerid]) return;
    gTunPonovi[playerid] = sad + (smjer == gTunSmjer[playerid] ? TUNING_PONOVI_MS : TUNING_CEKAJ_MS);
    gTunSmjer[playerid] = smjer;
    TuningPomakni(playerid, smjer);
}

TuningUlaz(playerid)
{
    if (!gUlogiran[playerid] || gTunU[playerid] != -1 || GetPlayerState(playerid) != PLAYER_STATE_DRIVER || GetPlayerVirtualWorld(playerid)) return 0;
    for (new g = 0; g < TUNING_MAX; g++)
    {
        if (!gTuning[g][tgIma] || !IsPlayerInRangeOfPoint(playerid, TUNING_DOMET, gTuning[g][tgX], gTuning[g][tgY], gTuning[g][tgZ])) continue;
        new v = GetPlayerVehicleID(playerid);
        if (!VlasnikDb(v) || VlasnikVozila(v) != gAccountId[playerid]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Tuning je dostupan samo za vlastita vozila.")), 1;
        TuningOtvori(playerid, g);
        return 1;
    }
    return 0;
}

ShowTuningAdmin(playerid)
{
    new list[512], row[96], len = 0;
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Nova tuning garaza na mojoj poziciji"));
    for (new g = 0; g < TUNING_MAX; g++)
    {
        if (!gTuning[g][tgIma]) continue;
        format(row, sizeof row, "[%d] %s", gTuning[g][tgDb], CestaZona(gTuning[g][tgX], gTuning[g][tgY]));
        DodajStupac(list, len, row);
    }
    return PrikaziDialog(playerid, DLG_APANEL_TUNING, DIALOG_STYLE_LIST, T(playerid, "Tuning garaze"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

static TuningNova(playerid)
{
    new g = 0;
    while (g < TUNING_MAX && gTuning[g][tgIma]) g++;
    if (g == TUNING_MAX || GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Tuning garaza se postavlja samo izvana."));
    new v = GetPlayerVehicleID(playerid);
    if (v)
    {
        GetVehiclePos(v, gTuning[g][tgX], gTuning[g][tgY], gTuning[g][tgZ]);
        GetVehicleZAngle(v, gTuning[g][tgA]);
    }
    else
    {
        GetPlayerPos(playerid, gTuning[g][tgX], gTuning[g][tgY], gTuning[g][tgZ]);
        GetPlayerFacingAngle(playerid, gTuning[g][tgA]);
    }
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO tuning_garages (x, y, z, a) VALUES (%f, %f, %f, %f)", gTuning[g][tgX], gTuning[g][tgY], gTuning[g][tgZ], gTuning[g][tgA]));
    gTuning[g][tgDb] = ZadnjiId();
    TuningStvori(g);
    return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Tuning garaza dodana: %s."), CestaZona(gTuning[g][tgX], gTuning[g][tgY]));
}

DLG:apanel_tuning(playerid, response, listitem)
{
    if (!response) return ShowKategoriju(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem == 0)
    {
        TuningNova(playerid);
        return ShowTuningAdmin(playerid);
    }
    new n = listitem;
    for (new g = 0; g < TUNING_MAX; g++)
    {
        if (!gTuning[g][tgIma] || --n) continue;
        gApTuning[playerid] = g;
        new body[96];
        format(body, sizeof body, T(playerid, "Obrisati tuning garazu: %s?"), CestaZona(gTuning[g][tgX], gTuning[g][tgY]));
        return PrikaziDialog(playerid, DLG_APANEL_TUNING_BRISI, DIALOG_STYLE_MSGBOX, T(playerid, "Tuning garaze"), body, T(playerid, "Obrisi"), T(playerid, "Natrag"));
    }
    return ShowTuningAdmin(playerid);
}

DLG:apanel_tuning_brisi(playerid, response)
{
    new g = gApTuning[playerid];
    if (response && ProvjeriOvlasti(playerid, ALVL_ORG) && g >= 0 && g < TUNING_MAX && gTuning[g][tgIma])
    {
        for (new i = 0; i < MAX_PLAYERS; i++) if (gTunU[i] == g) TuningZatvori(i);
        DestroyDynamic3DTextLabel(gTuning[g][tgTekst]);
        DestroyDynamicMapIcon(gTuning[g][tgIkona]);
        gTuning[g][tgIma] = false;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM tuning_garages WHERE id = %d", gTuning[g][tgDb]));
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Tuning garaza obrisana."));
    }
    return ShowTuningAdmin(playerid);
}
