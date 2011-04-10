/*=========================================================================
	
	�퓬�^�X�N��b

	�l�b�g���[�N�Ή��E�l�b�g���[�N��Ή� �̗����̐퓬�^�X�N�̋��ʕ���

===========================================================================*/

#include "stdafx.h"
#include "define_const.h"
#include "define_macro.h"
#include "global.h"
#include "battleTaskBase.h"
#include "task_loading.h"

#include "adaptor\define_data.h"	// �����E����
#include "adaptor\define_char.h"
#include "adaptor\mimikaki.h"
#include "adaptor\adaptor.h"


CBattleTaskBase::CBattleTaskBase()
{
	int i,j;
	for(j=0;j<2;j++){
		for(i=0;i<3;i++){
			hlib_c[j][i]=NULL;
		}
	}
	hlib_s=NULL;
	dsb_round = NULL;
	dsb_fight = NULL;
	dsb_ko = NULL;
}

/*==========================================================================


	�^�X�N Initialize


============================================================================*/

void CBattleTaskBase::Initialize()
{
	g_system.PushSysTag(__FUNCTION__);

	//[ FIGHT / KO ]�̃T�E���h�����[�h
	dsb_fight = g_sound.CreateDSB(".\\system\\sound\\fight.wav");
	dsb_ko = g_sound.CreateDSB(".\\system\\sound\\ko.wav");

	InitializeParameters();
	InitializeObjectList();
	InitializeDLLLoadInfo();
	InitializeLoadDLLs();
	InitializeSubTasks();

	g_system.PopSysTag();
}


//DLL�ɓn���\���̂̃p�����[�^������������
void CBattleTaskBase::InitializeDLLLoadInfo()
{
	UINT i,j;

	for(j=0;j<2;j++)
	{
		for(i=0;i<g_battleinfo.GetNumTeam(j);i++){
			strcpy(m_cinfo[j][i].dir,g_charlist.GetCharacterDir(g_battleinfo.GetCharacter(j,i)));
			m_cinfo[j][i].tid = j;
			m_cinfo[j][i].funcs = &g_exp.fpack_s;
			m_cinfo[j][i].funco = &g_exp.fpack_o;
			m_cinfo[j][i].funcd = &g_exp.fpack_d;
			m_cinfo[j][i].color = g_battleinfo.GetColor(j,i);
			m_cinfo[j][i].keyinput = g_battleinfo.GetKeyAssign(j,i);
			m_cinfo[j][i].options_flag = g_battleinfo.GetCharacterOption(j,i);
		}
	}

	m_sinfo.funcs = &g_exp.fpack_s;
	m_sinfo.funco = &g_exp.fpack_o;
	m_sinfo.funcd = &g_exp.fpack_d;
	m_sinfo.tid = TEAM_STAGE;
	strcpy(m_sinfo.dir,g_stagelist.GetStageDir(g_battleinfo.GetStage()));
}


void CBattleTaskBase::InitializeLoadDLLs()
{
	int i,j;
	char filename [MAX_PATH];

	CTNowLoading* now_loading = dynamic_cast<CTNowLoading*>(g_system.FindTask('LOAD'));

	//�L�����N�^�[dll�̃��[�h
	PFUNC_CREATECHARACTER pf_create=NULL;

	ZeroMemory(charobjid, sizeof(charobjid));
	stgobjid = 0;
	for(j=0;j<2;j++)
	{
		switch(g_battleinfo.GetNumTeam(j))
		{
		case 3://3�l�ڂ����[�h����
			i=2;
			m_crnt_dllid = j*MAXNUM_TEAM + i+1;
			m_current_com_level = g_battleinfo.GetComLevel(j,i);
			if ( g_charlist.GetCharacterVer(g_battleinfo.GetCharacter(j, i)) < 680 )
			{
				// �A�_�v�^�[���K�v�i�����j
				CCharAdaptor* pca = new CCharAdaptor;

				hlib_c[j][i] = NULL;
				if (pca)
				{
					if ( (charobjid[j][i] = pca->CreateCharacter(&m_cinfo[j][i])) == 0)
						{ delete pca; return; }
				}
				else {return;}
			}
			else
			{
				sprintf(filename,"%s\\action.dll",
					g_charlist.GetCharacterDir(g_battleinfo.GetCharacter(j,i)));
				hlib_c[j][i] = LoadLibrary(filename);

				if(hlib_c[j][i] != NULL){
					pf_create = (PFUNC_CREATECHARACTER)GetProcAddress(hlib_c[j][i],"CreateCharacter");
					if(pf_create!=NULL){
						charobjid[j][i] = (*pf_create)(&m_cinfo[j][i]);
					}
					else {return;}
				}
				else {return;}
			}
			if(now_loading)now_loading->Proceed(NowLoading_DLL);
		case 2://2�l�ڂ����[�h����
			i=1;
			m_crnt_dllid = j*MAXNUM_TEAM + i+1;
			m_current_com_level = g_battleinfo.GetComLevel(j,i);
			if ( g_charlist.GetCharacterVer(g_battleinfo.GetCharacter(j, i)) < 680 )
			{
				// �A�_�v�^�[���K�v�i�����j
				CCharAdaptor* pca = new CCharAdaptor;

				hlib_c[j][i] = NULL;
				if (pca)
				{
					if ( (charobjid[j][i] = pca->CreateCharacter(&m_cinfo[j][i])) == 0)
						{ delete pca; return; }
				}
				else {return;}
			}
			else
			{
				sprintf(filename,"%s\\action.dll",
					g_charlist.GetCharacterDir(g_battleinfo.GetCharacter(j,i)));
				hlib_c[j][i] = LoadLibrary(filename);

				if(hlib_c[j][i] != NULL){
					pf_create = (PFUNC_CREATECHARACTER)GetProcAddress(hlib_c[j][i],"CreateCharacter");
					if(pf_create!=NULL){
						charobjid[j][i] = (*pf_create)(&m_cinfo[j][i]);
					}
					else {return;}
				}
				else {return;}
			}
			if(now_loading)now_loading->Proceed(NowLoading_DLL);
		case 1://1�l�ڂ����[�h����
			i=0;
			m_crnt_dllid = j*MAXNUM_TEAM + i+1;
			m_current_com_level = g_battleinfo.GetComLevel(j,i);
			if ( g_charlist.GetCharacterVer(g_battleinfo.GetCharacter(j, i)) < 680 )
			{
				// �A�_�v�^�[���K�v�i�����j
				CCharAdaptor* pca = new CCharAdaptor;

				hlib_c[j][i] = NULL;
				if (pca)
				{
					if ( (charobjid[j][i] = pca->CreateCharacter(&m_cinfo[j][i])) == 0)
						{ delete pca; return; }
				}
				else {return;}
			}
			else
			{
				sprintf(filename,"%s\\action.dll",
					g_charlist.GetCharacterDir(g_battleinfo.GetCharacter(j,i)));
				hlib_c[j][i] = LoadLibrary(filename);

				if(hlib_c[j][i] != NULL){
					pf_create = (PFUNC_CREATECHARACTER)GetProcAddress(hlib_c[j][i],"CreateCharacter");
					if(pf_create!=NULL){
						charobjid[j][i] = (*pf_create)(&m_cinfo[j][i]);
					}
					else {return;}
				}
				else {return;}
			}
			if(now_loading)now_loading->Proceed(NowLoading_DLL);
		}
	}
	m_current_com_level = g_config.GetDifficulty();

	//�X�e�[�WDLL�̃��[�h
	m_crnt_dllid = 2*MAXNUM_TEAM+1;
	sprintf(filename,"%s\\stage.dll",
		g_stagelist.GetStageDir(g_battleinfo.GetStage()));
	hlib_s = LoadLibrary(filename);
	if(hlib_s == NULL){
		hlib_s = LoadLibrary(".\\system\\defstg.dll");
		if(!hlib_s){
			g_system.Log("failed to load default stage DLL",SYSLOG_WARNING);
		}
		if(now_loading)now_loading->Proceed(NowLoading_DLL);
	}
	PFUNC_CREATESTAGE pf_create_s=NULL;
	pf_create_s = (PFUNC_CREATESTAGE)GetProcAddress(hlib_s,"CreateStage");
	if(pf_create_s!=NULL){
		stgobjid = (*pf_create_s)(&m_sinfo);
	}
	else{
		g_system.Log("failed to get stage DLL create function pointer",SYSLOG_WARNING);
		g_system.Log(g_stagelist.GetStageDir(g_battleinfo.GetStage()),SYSLOG_WARNING);
	}

	m_crnt_dllid = 0;

	gbl.ods("CBattleTaskBase::InitializeLoadDLLs , DLL Load Complete");
}



/*==========================================================================


	�^�X�N Terminate


============================================================================*/
void CBattleTaskBase::Terminate()
{
	g_system.PushSysTag(__FUNCTION__);

	TerminateObjectList();
	TerminateUnloadDLLs();
	TerminateDestroySubTasks();

	g_input.KeyLock(FALSE);

	g_system.PopSysTag();
}

//DLL�J��
void CBattleTaskBase::TerminateUnloadDLLs()
{
	g_system.PushSysTag(__FUNCTION__);

	int i,j/*,k,l*/;
	for(j=0;j<2;j++){
		for(i=0;i<3;i++){
			//dll�̊J���i�d�����Ӂj
			/*for(k=0;k<2;k++){
				for(l=0;l<3;l++){
					if(hlib_c[j][i]!=hlib_c[k][l]){*/	// �d���͎Q�ƃJ�E���g�łǂ��ɂ����Ă�݂���
						RELEASEDLL(hlib_c[j][i]);
					/*}
					else
						hlib_c[j][i]=NULL;
				}
			}*/
		}
	}
	RELEASEDLL(hlib_s);

	for(j=0;j<2;j++){
		for(i=0;i<3;i++){//id��0�ɂ��Ă����B0=�I�u�W�F�N�g����
			charobjid[j][i]=0;
		}
	}
	stgobjid=0;

	RELEASE(dsb_round);
	RELEASE(dsb_fight);
	RELEASE(dsb_ko);

	g_system.PopSysTag();
}




/*==========================================================================


	�I�u�W�F�N�g�T�[�r�X


============================================================================*/

/*--------------------------------------------------------------------------
	�����_���[�W
		�������Ŏg�p�����B���̃_���[�W�ł�KO����͍s���Ȃ��B
		�����_���[�W�Ŏ��Ɏ������ꍇ�́A�����̊J�����Ɏ��S���肪�s����B
		�i�������łȂ��Ƃ���KO������s���悤�ɂ��Ă݂܂����B�j
----------------------------------------------------------------------------*/
void CBattleTaskBase::AddDamage(DWORD oid,DWORD eoid,int x,int y)
{
	g_system.PushSysTag(__FUNCTION__);

	if(bf_state!=BFSTATE_FIGHTING){
		g_system.PopSysTag();
		return;
	}
	DWORD a_id = oid;
	DWORD k_id = eoid;

	CGObject *attacker=GetGObject(a_id);
	CGObject *higaisya=GetGObject(k_id);

	if(attacker==NULL || higaisya==NULL){
		g_system.PopSysTag();
		return;
	}

	attacker->Message(GOBJMSG_TOUCHC,eoid);//�Ƃ肠���������������Ƃ�ʒm

	//�������_���[�W�����R�s�[
	higaisya->data.atk2.info1 = attacker->data.atk;
	higaisya->data.atk2.oid = a_id;
	//�t���O��
	higaisya->data.atk2.flags = 0;
	//��ʒ[�ő���������߂����ǂ���
	if(higaisya->data.objtype & GOBJFLG_CLIPX){
		if(attacker->data.objtype & GOBJFLG_HANSAYOU){
			higaisya->data.atk2.flags |= ATKINFO2_ATTACKERBACK;
		}
	}
	//�̂�����̌���
	if(attacker->data.muki){
		if(attacker->data.atk->muki){
			higaisya->data.atk2.flags |= ATKINFO2_RIGHTBACK;
		}
	}
	else{
		if(!(attacker->data.atk->muki)){
			higaisya->data.atk2.flags |= ATKINFO2_RIGHTBACK;
		}
	}

	DWORD i;
	GOBJECT		*pdat  =&(higaisya->data);
	ATTACKINFO  *aif = attacker->data.atk;
	MY2DVECTOR kas_point;
	kas_point.x =x;
	kas_point.y =y;

	double dmkanwa;
	if(TRUE){//��ɋ�炤
		dmkanwa=1.0;//�_���[�W�ɘa��
		//hit count
		if(pdat->aid&ACTID_KURAI){
			higaisya->hitcount++;
			if(higaisya->data.id == charobjid[higaisya->data.tid][active_character[higaisya->data.tid]]){//�A���q�b�g�\��
				if(higaisya->hitcount==2)
					bf_hitdisp[pdat->tid]=0;
				else bf_hitdisp[pdat->tid]=30;
			}
		}
		else{
			higaisya->hitcount=1;
			if(higaisya->data.id == charobjid[higaisya->data.tid][active_character[higaisya->data.tid]]){//�A���q�b�g�\��
				bf_hitdisp[pdat->tid]=0;
			}
		}
		for(i=1;i<higaisya->hitcount;i++){
			dmkanwa*=RDMGKANWA;
		}
		if(dmkanwa<DMGKANWA_MINIMUM)dmkanwa=DMGKANWA_MINIMUM;

		if(bf_state==BFSTATE_FIGHTING)pdat->hp -= (DWORD)(pdat->atk2.info1->damage * dmkanwa);
		if(higaisya->hitcount == 1)
			higaisya->sexydamage  = (DWORD)(pdat->atk2.info1->damage * dmkanwa);
		else if(higaisya->hitcount>=2){
			higaisya->sexydamage += (DWORD)(pdat->atk2.info1->damage * dmkanwa);

			if(higaisya->hitcount == 2)		// �\���p���ݒ�
				higaisya->sexydamage_anim = higaisya->sexydamage;
			else							// ���ݒ�
				higaisya->sexydamage_haba = abs((int)higaisya->sexydamage - (int)higaisya->sexydamage_anim) / 8;
		}

		switch(aif->hit & 0x000F0000){//�q�b�g�}�[�N�`��
		case 0:break;
		case HITINFO_MARK1:AddEffect(EFCTID_MARK1,(int)kas_point.x,(int)kas_point.y);break;
		case HITINFO_MARK2:AddEffect(EFCTID_MARK2,(int)kas_point.x,(int)kas_point.y);break;
		case HITINFO_MARK3:AddEffect(EFCTID_MARK3,(int)kas_point.x,(int)kas_point.y);break;
		case HITINFO_MARK4:AddEffect(EFCTID_MARK4,(int)kas_point.x,(int)kas_point.y);break;
		}
		switch(aif->hit & 0x00F00000){//���ʉ�
		case 0:break;
		case HITINFO_SNDHIT1:g_system.PlaySystemSound(SYSTEMSOUND_HIT1);break;
		case HITINFO_SNDHIT2:g_system.PlaySystemSound(SYSTEMSOUND_HIT2);break;
		case HITINFO_SNDHIT3:g_system.PlaySystemSound(SYSTEMSOUND_HIT3);break;
		case HITINFO_SNDSHK1:g_system.PlaySystemSound(SYSTEMSOUND_SHOCK1);break;
		case HITINFO_SNDSHK2:g_system.PlaySystemSound(SYSTEMSOUND_SHOCK2);break;
		default:CSystem::Log("��������Ă��Ȃ��q�b�g���ʉ����w�肳�ꂽ",SYSLOG_WARNING);
		}
		switch(aif->hit & 0x0F000000){//�q�b�g�X�g�b�v
		case 0:break;
		case HITINFO_SIV1:HitStop( 3,k_id);break;
		case HITINFO_SIV2:HitStop( 5,k_id);break;
		case HITINFO_SIV3:HitStop(10,k_id);break;
		case HITINFO_STOP:HitStop(40,k_id);break;
		default:CSystem::Log("��������Ă��Ȃ��q�b�g�X�g�b�vID",SYSLOG_WARNING);
		}
		if(pdat->aid != ACTID_NAGERARE && pdat->hp<=0){//���S
			if(g_battleinfo.GetBattleType()==TAISENKEISIKI_GOCYAMAZE)
				pdat->aid=ACTID_FINALDOWN;
			else
				pdat->aid=ACTID_KAITENFINISH;
			if(aif->hit & HITINFO_EFCTBURN )AddEffect(EFCTID_BURN,0,0,k_id);
			if(aif->hit & HITINFO_EFCTBURN_B )AddEffect(EFCTID_BURN_B,0,0,k_id);
			if(aif->hit & HITINFO_EFCTBURN_G )AddEffect(EFCTID_BURN_G,0,0,k_id);
		}
		//�G�t�F�N�g
		if((aif->hit & 0x0000F000) & HITINFO_EFCTSINDO)AddEffect(EFCTID_SINDO,2,20);
		if((aif->hit & 0x0000F000) & HITINFO_EFCTBURN  )AddEffect(EFCTID_BURN  ,0,0,k_id);
		if((aif->hit & 0x0000F000) & HITINFO_EFCTBURN_B)AddEffect(EFCTID_BURN_B,0,0,k_id);
		if((aif->hit & 0x0000F000) & HITINFO_EFCTBURN_G)AddEffect(EFCTID_BURN_G,0,0,k_id);
		
		//�U���������������Ƃ��U��������ɒʒm
		attacker->Message(GOBJMSG_TOUCHB,TRUE);
	}
	g_system.PopSysTag();
}



/*--------------------------------------------------------------------------
	�ԍ����擾
	�~���҂̉�]���l�����Ă��Ȃ�
	�~�I�u�W�F�N�g�̍��W�w�肪�f�B�X�v���C���W�������ꍇ���l�����Ă��Ȃ�
----------------------------------------------------------------------------*/

//���ԍ����擾�֐� �ɕK�v
inline void koukan(int *a,int *b)
{
	int tmp=*a;
	*a=*b;
	*b=tmp;
}

DWORD CBattleTaskBase::GetMaai(DWORD oid,DWORD eoid,BOOL h)
{
	GOBJECT *pdat = (GOBJECT*)GetGObjectInfo(oid);
	GOBJECT *pedat = (GOBJECT*)GetGObjectInfo(eoid);
	if(pdat==NULL)return(99999);
	if(pdat->phdat==NULL)return(99999);
	if(pedat==NULL)return(99999);
	if(pedat->phdat==NULL)return(99999);

	int i;
	int min_p1=99999,max_p1=-99999,min_p2=99999,max_p2=-99999;
	int hm2=99999,vm2=99999;

	if(!h){//�����ԍ���
		for(i=0;i<3;i++){
			if(min_p1 > pdat->phdat[ pdat->cnow ].kas[i].left)
				min_p1 = pdat->phdat[ pdat->cnow ].kas[i].left;
			if(max_p1 < pdat->phdat[ pdat->cnow ].kas[i].right)
				max_p1 = pdat->phdat[ pdat->cnow ].kas[i].right;
			if(min_p2 > pedat->phdat[ pedat->cnow ].kas[i].left)
				min_p2 = pedat->phdat[ pedat->cnow ].kas[i].left;
			if(max_p2 < pedat->phdat[ pedat->cnow ].kas[i].right)
				max_p2 = pedat->phdat[ pedat->cnow ].kas[i].right;
		}
		if(max_p1<min_p1)koukan(&min_p1,&max_p1);
		if(max_p2<min_p2)koukan(&min_p2,&max_p2);
		min_p1 += (int)pdat->x;
		max_p1 += (int)pdat->x;
		min_p2 += (int)pedat->x;
		max_p2 += (int)pedat->x;

		if(pdat->x > pedat->x)hm2 = min_p1 - max_p2;
		else                  hm2 = min_p2 - max_p1;
		if(hm2<0)hm2=0;
		return(hm2);
	}
	else {//�����ԍ���
		for(i=0;i<3;i++){
			if(min_p1 > pdat->phdat[ pdat->cnow ].kas[i].top)
				min_p1 = pdat->phdat[ pdat->cnow ].kas[i].top;
			if(max_p1 < pdat->phdat[ pdat->cnow ].kas[i].bottom)
				max_p1 = pdat->phdat[ pdat->cnow ].kas[i].bottom;
			if(min_p2 > pedat->phdat[ pedat->cnow ].kas[i].top)
				min_p2 = pedat->phdat[ pedat->cnow ].kas[i].top;
			if(max_p2 < pedat->phdat[ pedat->cnow ].kas[i].bottom)
				max_p2 = pedat->phdat[ pedat->cnow ].kas[i].bottom;
		}
		if(max_p1<min_p1)koukan(&min_p1,&max_p1);
		if(max_p2<min_p2)koukan(&min_p2,&max_p2);
		min_p1 += (int)pdat->y;
		max_p1 += (int)pdat->y;
		min_p2 += (int)pedat->y;
		max_p2 += (int)pedat->y;
		if(pdat->y > pedat->y)vm2 = min_p1 - max_p2;
		else                  vm2 = min_p2 - max_p1;
		if(vm2<0)vm2=0;
		return(vm2);
	}
}



/*--------------------------------------------------------------------------
	�I�u�W�F�N�gID����A�I�u�W�F�N�g���擾
----------------------------------------------------------------------------*/
GOBJECT* CBattleTaskBase::GetGObjectInfo(DWORD oid)
{
	CGObject *pgobj = GetGObject(oid);
	if(pgobj==NULL)return(NULL);
	else return( &pgobj->data );
}

/*--------------------------------------------------------------------------
	�`�[��ID,�L�����N�^�C���f�b�N�X����A�I�u�W�F�N�g���擾
----------------------------------------------------------------------------*/
GOBJECT* CBattleTaskBase::GetCharacterInfo(DWORD j,DWORD i)
{
	CGObject *pobj = GetCharacterObject(j,i);
	if(pobj==NULL)return(NULL);
	return( &(pobj->data) );
}



/*==========================================================================


	���擾�i�ݒ�j�n �֐�


-==========================================================================*/

DWORD CBattleTaskBase::GetWinCount(DWORD tid)
{
	return(wincount[tid]);
}

DWORD CBattleTaskBase::GetStrikerCount(DWORD tid)
{
	return(strikercount[tid]);
}

//�����䎌��ݒ�
void CBattleTaskBase::SetKatiSerif(DWORD tid,char *serif)
{
	g_battleresult.SetKatiSerif(tid,serif);
}

GOBJECT* CBattleTaskBase::GetActiveCharacter(DWORD tid)
{
	if(!(tid==TEAM_PLAYER1 || tid==TEAM_PLAYER2))return(NULL);
	if(active_character[tid]>=MAXNUM_TEAM){
		g_system.Log("CBattleTaskBase::GetActiveCharacter , active_character�Ƀw���Ȓl���͂����Ă��܂�",SYSLOG_WARNING);
		return(NULL);
	}

	return( GetGObjectInfo( charobjid[tid][active_character[tid]] ) );
}

DWORD CBattleTaskBase::GetActiveCharacterID(DWORD team)
{
	if(team>=2){
		g_system.Log("CBattleTaskBase::GetActiveCharacterID, ����team�Ƀw���Ȓl���w�肳��܂���",SYSLOG_WARNING);
		return 0;
	}
	if(active_character[team]>=MAXNUM_TEAM){
		g_system.Log("CBattleTaskBase::GetActiveCharacterID , active_character�Ƀw���Ȓl���͂����Ă��܂�",SYSLOG_WARNING);
		return 0;
	}
	return active_character[team];
}


/*==========================================================================


	�`��T�[�r�X�n �֐�


-==========================================================================*/

//�Â��R�[�h�Ɩ�������܂����킹�邽�߂�define
#define sdds	g_system.GetSystemGraphicSurface()
#define srdat	g_system.GetSystemGraphicRect()
#define scdat	g_system.GetSystemGraphicCell()
#define OLDCELLDRAW(a,b,c,d,e,f,g,h,i) g_draw.CellDraw(d,f,e,c,a,b,i,0,g,h)
//�Â��R�[�h�Ɩ�������܂����킹�����ɂ���ɋ����Ɋg��k���@�\���˂����ނ��߂�define
#define OLDCELLDRAW_NISE(a,b,c,d,e,f,g,h,i,j,k) g_draw.CellDraw(d,f,e,c,a,b,i,0,g,h,-1,j,k)

void CBattleTaskBase::DrawNumber(DWORD num,int x,int y,BOOL hits,float z, float magx, float magy)
{
	switch((num/10)%10){
	case 1:OLDCELLDRAW_NISE(x,y,CELL_NUMBER1,sdds,srdat,scdat,FALSE,FALSE,z,magx,magy);x+=(int)(20 * magx);break;
	case 2:OLDCELLDRAW_NISE(x,y,CELL_NUMBER2,sdds,srdat,scdat,FALSE,FALSE,z,magx,magy);x+=(int)(20 * magx);break;
	case 3:OLDCELLDRAW_NISE(x,y,CELL_NUMBER3,sdds,srdat,scdat,FALSE,FALSE,z,magx,magy);x+=(int)(20 * magx);break;
	case 4:OLDCELLDRAW_NISE(x,y,CELL_NUMBER4,sdds,srdat,scdat,FALSE,FALSE,z,magx,magy);x+=(int)(20 * magx);break;
	case 5:OLDCELLDRAW_NISE(x,y,CELL_NUMBER5,sdds,srdat,scdat,FALSE,FALSE,z,magx,magy);x+=(int)(20 * magx);break;
	case 6:OLDCELLDRAW_NISE(x,y,CELL_NUMBER6,sdds,srdat,scdat,FALSE,FALSE,z,magx,magy);x+=(int)(20 * magx);break;
	case 7:OLDCELLDRAW_NISE(x,y,CELL_NUMBER7,sdds,srdat,scdat,FALSE,FALSE,z,magx,magy);x+=(int)(20 * magx);break;
	case 8:OLDCELLDRAW_NISE(x,y,CELL_NUMBER8,sdds,srdat,scdat,FALSE,FALSE,z,magx,magy);x+=(int)(20 * magx);break;
	case 9:OLDCELLDRAW_NISE(x,y,CELL_NUMBER9,sdds,srdat,scdat,FALSE,FALSE,z,magx,magy);x+=(int)(20 * magx);break;
	case 0:
		if(!hits)
			OLDCELLDRAW_NISE(x,y,CELL_NUMBER0,sdds,srdat,scdat,FALSE,FALSE,z,magx,magy);x+=(int)(20 * magx);break;
	}
	switch(num%10){
	case 1:OLDCELLDRAW_NISE(x,y,CELL_NUMBER1,sdds,srdat,scdat,FALSE,FALSE,z,magx,magy);break;
	case 2:OLDCELLDRAW_NISE(x,y,CELL_NUMBER2,sdds,srdat,scdat,FALSE,FALSE,z,magx,magy);break;
	case 3:OLDCELLDRAW_NISE(x,y,CELL_NUMBER3,sdds,srdat,scdat,FALSE,FALSE,z,magx,magy);break;
	case 4:OLDCELLDRAW_NISE(x,y,CELL_NUMBER4,sdds,srdat,scdat,FALSE,FALSE,z,magx,magy);break;
	case 5:OLDCELLDRAW_NISE(x,y,CELL_NUMBER5,sdds,srdat,scdat,FALSE,FALSE,z,magx,magy);break;
	case 6:OLDCELLDRAW_NISE(x,y,CELL_NUMBER6,sdds,srdat,scdat,FALSE,FALSE,z,magx,magy);break;
	case 7:OLDCELLDRAW_NISE(x,y,CELL_NUMBER7,sdds,srdat,scdat,FALSE,FALSE,z,magx,magy);break;
	case 8:OLDCELLDRAW_NISE(x,y,CELL_NUMBER8,sdds,srdat,scdat,FALSE,FALSE,z,magx,magy);break;
	case 9:OLDCELLDRAW_NISE(x,y,CELL_NUMBER9,sdds,srdat,scdat,FALSE,FALSE,z,magx,magy);break;
	case 0:OLDCELLDRAW_NISE(x,y,CELL_NUMBER0,sdds,srdat,scdat,FALSE,FALSE,z,magx,magy);break;
	}
	if(hits){
		x+=(int)(20 * magx);
		OLDCELLDRAW_NISE(x,y,CELL_HITS,sdds,srdat,scdat,FALSE,FALSE,z,magx,magy);
	}

}

void CBattleTaskBase::DrawNumber2(DWORD num,int x,int y,float z)
{
	OLDCELLDRAW(x,y+5,CELL_DAMAGE,sdds,srdat,scdat,FALSE,FALSE,z);

	int zure1=20;
	BOOL uketa = FALSE;
	x+=40;

	switch((num/1000)%10){
	case 1:OLDCELLDRAW(x,y,CELL_NUMBER1,sdds,srdat,scdat,FALSE,FALSE,z);x+=zure1;uketa=TRUE;break;
	case 2:OLDCELLDRAW(x,y,CELL_NUMBER2,sdds,srdat,scdat,FALSE,FALSE,z);x+=zure1;uketa=TRUE;break;
	case 3:OLDCELLDRAW(x,y,CELL_NUMBER3,sdds,srdat,scdat,FALSE,FALSE,z);x+=zure1;uketa=TRUE;break;
	case 4:OLDCELLDRAW(x,y,CELL_NUMBER4,sdds,srdat,scdat,FALSE,FALSE,z);x+=zure1;uketa=TRUE;break;
	case 5:OLDCELLDRAW(x,y,CELL_NUMBER5,sdds,srdat,scdat,FALSE,FALSE,z);x+=zure1;uketa=TRUE;break;
	case 6:OLDCELLDRAW(x,y,CELL_NUMBER6,sdds,srdat,scdat,FALSE,FALSE,z);x+=zure1;uketa=TRUE;break;
	case 7:OLDCELLDRAW(x,y,CELL_NUMBER7,sdds,srdat,scdat,FALSE,FALSE,z);x+=zure1;uketa=TRUE;break;
	case 8:OLDCELLDRAW(x,y,CELL_NUMBER8,sdds,srdat,scdat,FALSE,FALSE,z);x+=zure1;uketa=TRUE;break;
	case 9:OLDCELLDRAW(x,y,CELL_NUMBER9,sdds,srdat,scdat,FALSE,FALSE,z);x+=zure1;uketa=TRUE;break;
	}
	switch((num/100)%10){
	case 1:OLDCELLDRAW(x,y,CELL_NUMBER1,sdds,srdat,scdat,FALSE,FALSE,z);x+=zure1;uketa=TRUE;break;
	case 2:OLDCELLDRAW(x,y,CELL_NUMBER2,sdds,srdat,scdat,FALSE,FALSE,z);x+=zure1;uketa=TRUE;break;
	case 3:OLDCELLDRAW(x,y,CELL_NUMBER3,sdds,srdat,scdat,FALSE,FALSE,z);x+=zure1;uketa=TRUE;break;
	case 4:OLDCELLDRAW(x,y,CELL_NUMBER4,sdds,srdat,scdat,FALSE,FALSE,z);x+=zure1;uketa=TRUE;break;
	case 5:OLDCELLDRAW(x,y,CELL_NUMBER5,sdds,srdat,scdat,FALSE,FALSE,z);x+=zure1;uketa=TRUE;break;
	case 6:OLDCELLDRAW(x,y,CELL_NUMBER6,sdds,srdat,scdat,FALSE,FALSE,z);x+=zure1;uketa=TRUE;break;
	case 7:OLDCELLDRAW(x,y,CELL_NUMBER7,sdds,srdat,scdat,FALSE,FALSE,z);x+=zure1;uketa=TRUE;break;
	case 8:OLDCELLDRAW(x,y,CELL_NUMBER8,sdds,srdat,scdat,FALSE,FALSE,z);x+=zure1;uketa=TRUE;break;
	case 9:OLDCELLDRAW(x,y,CELL_NUMBER9,sdds,srdat,scdat,FALSE,FALSE,z);x+=zure1;uketa=TRUE;break;
	default:
		if(uketa)OLDCELLDRAW(x,y,CELL_NUMBER0,sdds,srdat,scdat,FALSE,FALSE,z);x+=zure1;
	}
	switch((num/10)%10){
	case 1:OLDCELLDRAW(x,y,CELL_NUMBER1,sdds,srdat,scdat,FALSE,FALSE,z);x+=zure1;break;
	case 2:OLDCELLDRAW(x,y,CELL_NUMBER2,sdds,srdat,scdat,FALSE,FALSE,z);x+=zure1;break;
	case 3:OLDCELLDRAW(x,y,CELL_NUMBER3,sdds,srdat,scdat,FALSE,FALSE,z);x+=zure1;break;
	case 4:OLDCELLDRAW(x,y,CELL_NUMBER4,sdds,srdat,scdat,FALSE,FALSE,z);x+=zure1;break;
	case 5:OLDCELLDRAW(x,y,CELL_NUMBER5,sdds,srdat,scdat,FALSE,FALSE,z);x+=zure1;break;
	case 6:OLDCELLDRAW(x,y,CELL_NUMBER6,sdds,srdat,scdat,FALSE,FALSE,z);x+=zure1;break;
	case 7:OLDCELLDRAW(x,y,CELL_NUMBER7,sdds,srdat,scdat,FALSE,FALSE,z);x+=zure1;break;
	case 8:OLDCELLDRAW(x,y,CELL_NUMBER8,sdds,srdat,scdat,FALSE,FALSE,z);x+=zure1;break;
	case 9:OLDCELLDRAW(x,y,CELL_NUMBER9,sdds,srdat,scdat,FALSE,FALSE,z);x+=zure1;break;
	default:
		if(uketa)OLDCELLDRAW(x,y,CELL_NUMBER0,sdds,srdat,scdat,FALSE,FALSE,z);x+=zure1;
	}
	switch(num%10){
	case 1:OLDCELLDRAW(x,y,CELL_NUMBER1,sdds,srdat,scdat,FALSE,FALSE,z);break;
	case 2:OLDCELLDRAW(x,y,CELL_NUMBER2,sdds,srdat,scdat,FALSE,FALSE,z);break;
	case 3:OLDCELLDRAW(x,y,CELL_NUMBER3,sdds,srdat,scdat,FALSE,FALSE,z);break;
	case 4:OLDCELLDRAW(x,y,CELL_NUMBER4,sdds,srdat,scdat,FALSE,FALSE,z);break;
	case 5:OLDCELLDRAW(x,y,CELL_NUMBER5,sdds,srdat,scdat,FALSE,FALSE,z);break;
	case 6:OLDCELLDRAW(x,y,CELL_NUMBER6,sdds,srdat,scdat,FALSE,FALSE,z);break;
	case 7:OLDCELLDRAW(x,y,CELL_NUMBER7,sdds,srdat,scdat,FALSE,FALSE,z);break;
	case 8:OLDCELLDRAW(x,y,CELL_NUMBER8,sdds,srdat,scdat,FALSE,FALSE,z);break;
	case 9:OLDCELLDRAW(x,y,CELL_NUMBER9,sdds,srdat,scdat,FALSE,FALSE,z);break;
	case 0:OLDCELLDRAW(x,y,CELL_NUMBER0,sdds,srdat,scdat,FALSE,FALSE,z);break;
	}
	x+=zure1;
	OLDCELLDRAW(x,y,CELL_PTS,sdds,srdat,scdat,FALSE,FALSE,z);
}

void CBattleTaskBase::DrawNumber3(double numd,int x,int y,float z)//�Q�[�W1�p
{
	int num=(int)numd;

	switch(num%10){
	case 1:OLDCELLDRAW(x,y,CELL_PG2_NUM1,sdds,srdat,scdat,FALSE,FALSE,z);break;
	case 2:OLDCELLDRAW(x,y,CELL_PG2_NUM2,sdds,srdat,scdat,FALSE,FALSE,z);break;
	case 3:OLDCELLDRAW(x,y,CELL_PG2_NUM3,sdds,srdat,scdat,FALSE,FALSE,z);break;
	case 4:OLDCELLDRAW(x,y,CELL_PG2_NUM4,sdds,srdat,scdat,FALSE,FALSE,z);break;
	case 5:OLDCELLDRAW(x,y,CELL_PG2_NUM5,sdds,srdat,scdat,FALSE,FALSE,z);break;
	case 6:OLDCELLDRAW(x,y,CELL_PG2_NUM6,sdds,srdat,scdat,FALSE,FALSE,z);break;
	case 7:OLDCELLDRAW(x,y,CELL_PG2_NUM7,sdds,srdat,scdat,FALSE,FALSE,z);break;
	case 8:OLDCELLDRAW(x,y,CELL_PG2_NUM8,sdds,srdat,scdat,FALSE,FALSE,z);break;
	case 9:OLDCELLDRAW(x,y,CELL_PG2_NUM9,sdds,srdat,scdat,FALSE,FALSE,z);break;
	case 0:OLDCELLDRAW(x,y,CELL_PG2_NUM0,sdds,srdat,scdat,FALSE,FALSE,z);break;
	}
}

void CBattleTaskBase::DrawNumber4(double numd,int x,int y,float z)//�Q�[�W2�p
{
	int num = (int)(numd*100.0);

	switch((num/10)%10){
	case 1:OLDCELLDRAW(x,y,CELL_PG1_NUM1,sdds,srdat,scdat,FALSE,FALSE,z);break;
	case 2:OLDCELLDRAW(x,y,CELL_PG1_NUM2,sdds,srdat,scdat,FALSE,FALSE,z);break;
	case 3:OLDCELLDRAW(x,y,CELL_PG1_NUM3,sdds,srdat,scdat,FALSE,FALSE,z);break;
	case 4:OLDCELLDRAW(x,y,CELL_PG1_NUM4,sdds,srdat,scdat,FALSE,FALSE,z);break;
	case 5:OLDCELLDRAW(x,y,CELL_PG1_NUM5,sdds,srdat,scdat,FALSE,FALSE,z);break;
	case 6:OLDCELLDRAW(x,y,CELL_PG1_NUM6,sdds,srdat,scdat,FALSE,FALSE,z);break;
	case 7:OLDCELLDRAW(x,y,CELL_PG1_NUM7,sdds,srdat,scdat,FALSE,FALSE,z);break;
	case 8:OLDCELLDRAW(x,y,CELL_PG1_NUM8,sdds,srdat,scdat,FALSE,FALSE,z);break;
	case 9:OLDCELLDRAW(x,y,CELL_PG1_NUM9,sdds,srdat,scdat,FALSE,FALSE,z);break;
	case 0:OLDCELLDRAW(x,y,CELL_PG1_NUM0,sdds,srdat,scdat,FALSE,FALSE,z);break;
	}

	x+=12;

	switch(num%10){
	case 1:OLDCELLDRAW(x,y,CELL_PG1_NUM1,sdds,srdat,scdat,FALSE,FALSE,z);break;
	case 2:OLDCELLDRAW(x,y,CELL_PG1_NUM2,sdds,srdat,scdat,FALSE,FALSE,z);break;
	case 3:OLDCELLDRAW(x,y,CELL_PG1_NUM3,sdds,srdat,scdat,FALSE,FALSE,z);break;
	case 4:OLDCELLDRAW(x,y,CELL_PG1_NUM4,sdds,srdat,scdat,FALSE,FALSE,z);break;
	case 5:OLDCELLDRAW(x,y,CELL_PG1_NUM5,sdds,srdat,scdat,FALSE,FALSE,z);break;
	case 6:OLDCELLDRAW(x,y,CELL_PG1_NUM6,sdds,srdat,scdat,FALSE,FALSE,z);break;
	case 7:OLDCELLDRAW(x,y,CELL_PG1_NUM7,sdds,srdat,scdat,FALSE,FALSE,z);break;
	case 8:OLDCELLDRAW(x,y,CELL_PG1_NUM8,sdds,srdat,scdat,FALSE,FALSE,z);break;
	case 9:OLDCELLDRAW(x,y,CELL_PG1_NUM9,sdds,srdat,scdat,FALSE,FALSE,z);break;
	case 0:OLDCELLDRAW(x,y,CELL_PG1_NUM0,sdds,srdat,scdat,FALSE,FALSE,z);break;
	}

	OLDCELLDRAW(x+12,y,CELL_PG1_PER,sdds,srdat,scdat,FALSE,FALSE,z);
}

void CBattleTaskBase::DrawGObject(GOBJECT* pdat)
{
	BOOL rev_x=FALSE;
	if(pdat->revx)rev_x=!rev_x;
	if(pdat->muki)rev_x=!rev_x;
	
	//�`��̐ݒ�
	if(pdat->alphamode!=0)g_draw.SetAlphaMode(pdat->alphamode);
	if(pdat->objtype & GOBJFLG_DISPZAHYO)SetTransform(FALSE);

	g_draw.CellDraw(pdat->pmsarr,pdat->pcdat,pdat->prdat,
		pdat->cnow,(int)pdat->x,(int)pdat->y,pdat->z,
		pdat->rot,rev_x,pdat->revy,pdat->color,pdat->magx,pdat->magy);

	//�`��̐ݒ�E���ɖ߂��Ă���
	if(pdat->alphamode!=0)g_draw.SetAlphaMode(0);
	if(pdat->objtype & GOBJFLG_DISPZAHYO)SetTransform(TRUE);
}


const char* CBattleTaskBase::MessageID2String(DWORD id)
{
	switch(id)
	{
	case GOBJMSG_DELETE			: return "GOBJMSG_DELETE";
	case GOBJMSG_ACTION			: return "GOBJMSG_ACTION";
	case GOBJMSG_COMMAND		: return "GOBJMSG_COMMAND";
	case GOBJMSG_COMMANDCOM		: return "GOBJMSG_COMMANDCOM";
	case GOBJMSG_TOUCHA			: return "GOBJMSG_TOUCHA";
	case GOBJMSG_TOUCHB			: return "GOBJMSG_TOUCHB";
	case GOBJMSG_TOUCHC			: return "GOBJMSG_TOUCHC";
	case GOBJMSG_CNGAID			: return "GOBJMSG_CNGAID";
	//�`��n���b�Z�[�W
	case GOBJMSG_DRAW			: return "GOBJMSG_DRAW";
	case GOBJMSG_DRAWBACK		: return "GOBJMSG_DRAWBACK";
	case GOBJMSG_DRAWFRONT		: return "GOBJMSG_DRAWFRONT";
	//�o��E���Ȃǃ��b�Z�[�W
	case GOBJMSG_DOTOJYO		: return "GOBJMSG_DOTOJYO";
	case GOBJMSG_DOTIMEOVERLOSE	: return "GOBJMSG_DOTIMEOVER";
	case GOBJMSG_DOYOUWIN		: return "GOBJMSG_DOYOUWIN";
	case GOBJMSG_TAIKI			: return "GOBJMSG_TAIKI";
	case GOBJMSG_KOUTAI			: return "GOBJMSG_KOUTAI";
	case GOBJMSG_KOUTAI2		: return "GOBJMSG_KOUTAI2";
	case GOBJMSG_STRIKER		: return "GOBJMSG_STRIKER";
	case GOBJMSG_DOYOUWIN2		: return "GOBJMSG_DOYOUWIN2";
	case GOBJMSG_STRIKERREADY	: return "GOBJMSG_STRIKERREADY";
	//���W����n���b�Z�[�W
	case GOBJMSG_KNOCKBACK		: return "GOBJMSG_KNOCKBACK";
	case GOBJMSG_CLIPX			: return "GOBJMSG_CLIPX";
	//�I�u�W�F�N�g�ԑ��ݍ�p�E���̑�
	case GOBJMSG_CNGTARGET		: return "GOBJMSG_CNGTARGET";
	case GOBJMSG_SOUSAI			: return "GOBJMSG_SOUSAI";
	//�Q�[���i�s
	case GOBJMSG_CNGROUND		: return "GOBJMSG_CNGGROUND";
	//�l�b�g���[�N
	case GOBJMSG_SYNC			: return "GOBJMSG_SYNC";
	}
	static char errret[64];
	sprintf(errret,"unknown-ID(0x%08X)",id);
	return errret;
}

void CBattleTaskBase::Notify_Exception(CGObject *obj,DWORD msgid,DWORD prm)
{
	UINT team ;
	UINT index;

	switch(obj->dll_id)
	{
	case 0:
		team = 3;//system
		break;
	case 1:
		team = 0;
		index = 0;
		break;
	case 2:
		team = 0;
		index = 1;
		break;
	case 3:
		team = 0;
		index = 2;
		break;
	case 4:
		team = 1;
		index = 0;
		break;
	case 5:
		team = 1;
		index = 1;
		break;
	case 6:
		team = 1;
		index = 2;
		break;
	case 7:
		team = 2;//stage
		break;
	default:team = 3;
	}

	if(team==3)
	{
		g_system.LogErr("system/unknown object , msg:%s prm:0x%08X act:0x%08X cnt:%d last_func:%s(%s)",
			MessageID2String(msgid),
			prm,
			obj->data.aid,
			obj->data.counter,
			CExport::last_funcname,
			CExport::func_in ? "in" : "out"
			);
	}
	else if(team==2)
	{
		UINT sindex = g_battleinfo.GetStage();

		g_system.LogErr("stage object %s(%s) , msg:%s prm:%08X act:0x%08X cnt:%d last_func:%s(%s)",
			g_stagelist.GetStageName(sindex),
			(stgobjid==obj->data.id) ? "main":"sub",
			MessageID2String(msgid),
			prm,
			obj->data.aid,
			obj->data.counter,
			CExport::last_funcname,
			CExport::func_in ? "in" : "out"
			);
	}
	else
	{
		UINT cindex = g_battleinfo.GetCharacter(team,index);

		g_system.LogErr("character object %s(%s) , msg:%s prm:0x%08X act:0x%08X cnt:%d last_func:%s(%s)",
			g_charlist.GetCharacterName(cindex),
			(charobjid[team][index]==obj->data.id) ? "main":"sub",
			MessageID2String(msgid),
			prm,
			obj->data.aid,
			obj->data.counter,
			CExport::last_funcname,
			CExport::func_in ? "in" : "out"
			);
	}
}


const char* CBattleTaskBase::GetBattleStateString()
{
	switch(bf_state)
	{
	case BFSTATE_WAITFORENDPOSE	:return "BFSTATE_WAITFORENDPOSE";
	case BFSTATE_ROUNDCALL		:return "BFSTATE_ROUNDCALL";
	case BFSTATE_FIGHTING		:return "BFSTATE_FIGHTING";
	case BFSTATE_FINISHED		:return "BFSTATE_FINISHED";
	case BFSTATE_WAITFORENDWIN	:return "BFSTATE_WAITFORENDWIN";
	case BFSTATE_DOUBLEKO		:return "BFSTATE_DOUBLEKO";
	case BFSTATE_TIMEOVER		:return "BFSTATE_TIMEOVER";
	}
	return "BFSTATE_UNKNOWN";
}

