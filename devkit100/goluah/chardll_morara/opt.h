
#include "gobject.h"

//�I�v�V������`(�������G��)
#define OPTIONS_CHAIN_COMBO_EL		0x00010000
#define OPTIONS_TETRAGRAMMATON		0x00020000
#define OPTIONS_SWORDOFJUSTICE		0x00040000
#define OPTIONS_DAYOFJUDGEMENT		0x00080000
#define OPTIONS_MEGIDORAON			0x00100000
#define OPTIONS_REGENERATOR			0x00200000
#define OPTIONS_ENERGY_SOURCE		0x00400000
#define OPTIONS_ENERGY_SOURCE2		0x00800000
#define OPTIONS_ATTACK30			0x01000000
#define OPTIONS_HP50				0x02000000
#define OPTIONS_AUTOGUARD			0x04000000

#define OPTIONS_MORARAEL_ALL		0x07FF0000//��L���ׂĂ̑g�ݍ��킹

//�I�v�V������`
#define OPTIONS_CHAIN_COMBO			(0x00000001)
#define OPTIONS_EXTRA_ATTACK		(0x00000002)
#define OPTIONS_AUTO_GUARD			(0x00000004)
#define OPTIONS_GUARD_CANCEL		(0x00000008)
#define OPTIONS_G_CUTTER_PLUS		(0x00000010)
#define OPTIONS_CHIBASIRI_PLUS		(0x00000020)
#define OPTIONS_2ND_STEP			(0x00000040)
#define OPTIONS_STOCKABLE_GAUGE		(0x00000080)
#define OPTIONS_MINIGUN_RELOADED	(0x00000100)

#define OPTIONS_MORARA_ALL			(0x000001FF)//��L���ׂĂ̑g�ݍ��킹

#define OPTIONS_MORARAEL_MODE		(0x08000000 | CHARACTER_LOAD_OPTION_FACE_A | OPTIONS_MORARAEL_ALL)
