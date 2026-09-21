#include "dlls/object_descriptor.h"
#include "dlls/objects/198_AnimatedObj.h"
#include "dlls/objects/199_DIM2RoofRub.h"
#include "dlls/objects/200_DepthOfFieldPoint.h"
#include "dlls/objects/202.h"
#include "dlls/objects/203.h"
#include "dlls/objects/204_ChukChuk.h"
#include "dlls/objects/205_IceBall.h"
#include "dlls/objects/206.h"
#include "dlls/objects/207_CannonClaw.h"
#include "dlls/objects/208_Grimble.h"
#include "dlls/objects/209_TumbleWeedB.h"
#include "dlls/objects/210.h"
#include "dlls/objects/212_SkeetlaWall.h"
#include "dlls/objects/213_Kaldachom.h"
#include "dlls/objects/214_KaldachomMe.h"
#include "dlls/objects/215.h"
#include "dlls/objects/216_PinPonSpike.h"
#include "dlls/objects/217_Pollen.h"
#include "dlls/objects/219_MikaBomb.h"
#include "dlls/objects/220_MikaBombShadow.h"
#include "dlls/objects/221_GCbaddieShield.h"
#include "dlls/objects/222_BaddieInterestP.h"
#include "dlls/objects/223_Hagabon.h"
#include "dlls/objects/224_SwarmBaddie.h"
#include "dlls/objects/225_WispBaddie.h"
#include "dlls/objects/227_Fireball.h"
#include "dlls/objects/228_FlameThrowerspe.h"
#include "dlls/objects/229_Shield.h"
#include "dlls/objects/230_ReStartMark.h"
#include "dlls/objects/231.h"
#include "dlls/objects/232_Checkpoint4.h"
#include "dlls/objects/233_Setuppoint.h"
#include "dlls/objects/234_Sideload.h"
#include "dlls/objects/235.h"
#include "dlls/objects/236_InfoPoint.h"
#include "dlls/objects/237.h"
#include "dlls/objects/238_EffectBox.h"
#include "dlls/objects/239.h"
#include "dlls/objects/240_WarpPoint.h"
#include "dlls/objects/241_InvHit.h"
#include "dlls/objects/242_iceblast.h"
#include "dlls/objects/243_flameblast.h"
#include "dlls/objects/244.h"
#include "dlls/objects/245_SidekickBal.h"
#include "dlls/objects/246_Area.h"
#include "dlls/objects/247.h"
#include "dlls/objects/248_LevelName.h"
#include "dlls/objects/249.h"
#include "dlls/objects/250_InvisibleHi.h"
#include "dlls/objects/251.h"
#include "dlls/objects/252.h"
#include "dlls/objects/253.h"
#include "dlls/objects/254_MagicPlant.h"
#include "dlls/objects/255.h"
#include "dlls/objects/256_TrickyWarp.h"
#include "dlls/objects/257_TrickyGuard.h"
#include "dlls/objects/258_StayPoint.h"
#include "dlls/objects/259_CurveFish.h"
#include "dlls/objects/260_SmallBasket.h"
#include "dlls/objects/261_LargeCrate.h"
#include "dlls/objects/262.h"
#include "dlls/objects/263.h"
#include "dlls/objects/264_EndObject.h"
#include "dlls/objects/265.h"
#include "dlls/objects/266_Fall_Ladder.h"
#include "dlls/objects/267_FireFlyLant.h"
#include "dlls/objects/268_LanternFire.h"
#include "dlls/objects/269_PortalSpell.h"
#include "dlls/objects/270.h"
#include "dlls/objects/271_MMP_Bridge.h"
#include "dlls/objects/272.h"
#include "dlls/objects/273.h"
#include "dlls/objects/274.h"
#include "dlls/objects/275.h"
#include "dlls/objects/276_IMMultiSeq.h"
#include "dlls/objects/277.h"
#include "dlls/objects/278_WM_Column.h"
#include "dlls/objects/279_AppleOnTree.h"
#include "dlls/objects/280_Duster.h"
#include "dlls/objects/281_coldWaterCo.h"
#include "dlls/objects/282.h"
#include "dlls/objects/283_Landed_Arwi.h"
#include "dlls/objects/284.h"
#include "dlls/objects/285.h"
#include "dlls/objects/286_MagicCaveBo.h"
#include "dlls/objects/287_MagicCaveTo.h"
#include "dlls/objects/288_TrickyGuard.h"
#include "dlls/objects/289.h"
#include "dlls/objects/290_CCTestInfot.h"
#include "dlls/objects/291_fuelCell.h"
#include "dlls/objects/292.h"
#include "dlls/objects/293_curve.h"
#include "dlls/objects/295.h"
#include "dlls/objects/296_KT_Torch.h"
#include "dlls/objects/297_CampFire.h"
#include "dlls/objects/298_CFCrate.h"
#include "dlls/objects/299_FXEmit.h"
#include "dlls/objects/300_Transporter.h"
#include "dlls/objects/301_LFXEmitter.h"
#include "dlls/objects/302.h"
#include "dlls/objects/303_BarrelPad.h"
#include "dlls/objects/304_AreaFXEmit.h"
#include "dlls/objects/305.h"
#include "dlls/objects/306_WaterFallSp.h"
#include "dlls/objects/307_sfxPlayer.h"
#include "dlls/objects/308_texscroll2.h"
#include "dlls/objects/309_texscroll.h"
#include "dlls/objects/310_WaveAnimato.h"
#include "dlls/objects/311_AlphaAnimat.h"
#include "dlls/objects/312_GroundAnima.h"
#include "dlls/objects/313_HitAnimator.h"
#include "dlls/objects/314_VisAnimator.h"
#include "dlls/objects/315_WallAnimato.h"
#include "dlls/objects/316_XYZAnimator.h"
#include "dlls/objects/317_ExplodeAnim.h"
#include "dlls/objects/318.h"
#include "dlls/objects/319_TexFrameAni.h"
#include "dlls/objects/320_fogControl.h"
#include "dlls/objects/321_Lightning.h"
#include "dlls/objects/322_FElevContro.h"
#include "dlls/objects/323_FEseqobject.h"
#include "dlls/objects/324.h"
#include "dlls/objects/325_CloudPrison.h"
#include "dlls/objects/328_CFGuardian.h"
#include "dlls/objects/329.h"
#include "dlls/objects/330_CFPowerBase.h"
#include "dlls/objects/331_CFMainCryst.h"
#include "dlls/objects/332.h"
#include "dlls/objects/334_CFPrisonGua.h"
#include "dlls/objects/335_CFPrisonUnc.h"
#include "dlls/objects/336_GCRobotLigh.h"
#include "dlls/objects/339_CFPerch.h"
#include "dlls/objects/340.h"
#include "dlls/objects/343_SpiritDoorS.h"
#include "dlls/objects/344.h"
#include "dlls/objects/345.h"
#include "dlls/objects/346.h"
#include "dlls/objects/347_CFForceFiel.h"
#include "dlls/objects/349.h"
#include "dlls/objects/351.h"
#include "dlls/objects/354_CFMagicWall.h"
#include "dlls/objects/356_CFLevelCont.h"
#include "dlls/objects/358.h"
#include "dlls/objects/359_SpiritDoorL.h"
#include "dlls/objects/361_IMIceMounta.h"
#include "dlls/objects/362_CRrockfall.h"
#include "dlls/objects/363.h"
#include "dlls/objects/364.h"
#include "dlls/objects/365_IMIcePillar.h"
#include "dlls/objects/366_IMAnimSpace.h"
#include "dlls/objects/367_IMSpaceThru.h"
#include "dlls/objects/368_IMSpaceRing.h"
#include "dlls/objects/369_IMSpaceRing.h"
#include "dlls/objects/370_LINKB_levco.h"
#include "dlls/objects/371_LINK_levcon.h"
#include "dlls/objects/372_CCriverflow.h"
#include "dlls/objects/373_DFropenode.h"
#include "dlls/objects/375.h"
#include "dlls/objects/376_DFSH_Shrine.h"
#include "dlls/objects/377_DFSH_ObjCre.h"
#include "dlls/objects/378_SpiritPrize.h"
#include "dlls/objects/379_DFSH_LaserB.h"
#include "dlls/objects/381.h"
#include "dlls/objects/382_MMP_levelco.h"
#include "dlls/objects/383.h"
#include "dlls/objects/384_MMP_asteroi.h"
#include "dlls/objects/385_MMP_trenchF.h"
#include "dlls/objects/386_MMP_moonroc.h"
#include "dlls/objects/387_MMP_gyserve.h"
#include "dlls/objects/388.h"
#include "dlls/objects/389_CCgasvent.h"
#include "dlls/objects/390_CCgasventCo.h"
#include "dlls/objects/391_CCqueen.h"
#include "dlls/objects/392_CClightfoot.h"
#include "dlls/objects/393_CCSharpclaw.h"
#include "dlls/objects/394_CCpedstal.h"
#include "dlls/objects/395_CClevcontro.h"
#include "dlls/objects/396_MMSH_Shrine.h"
#include "dlls/objects/397_MMSH_Scales.h"
#include "dlls/objects/398_MMSH_WaterS.h"
#include "dlls/objects/399_ECSH_Shrine.h"
#include "dlls/objects/400_ECSH_Cup.h"
#include "dlls/objects/401_ECSH_Creato.h"
#include "dlls/objects/402_GPSH_Shrine.h"
#include "dlls/objects/403_GPSH_ObjCre.h"
#include "dlls/objects/404_GPSH_Scene.h"
#include "dlls/objects/405_DBSH_Shrine.h"
#include "dlls/objects/406_DBSH_Symbol.h"
#include "dlls/objects/407.h"
#include "dlls/objects/408_NWSH_levcon.h"
#include "dlls/objects/409.h"
#include "dlls/objects/410.h"
#include "dlls/objects/411.h"
#include "dlls/objects/412.h"
#include "dlls/objects/413.h"
#include "dlls/objects/414.h"
#include "dlls/objects/415_NW_treebrid.h"
#include "dlls/objects/416_NW_geyser.h"
#include "dlls/objects/417_NW_mammoth.h"
#include "dlls/objects/418_NW_tricky.h"
#include "dlls/objects/419.h"
#include "dlls/objects/420.h"
#include "dlls/objects/421_NW_levcontr.h"
#include "dlls/objects/422_SH_tricky.h"
#include "dlls/objects/423.h"
#include "dlls/objects/424_SH_killermu.h"
#include "dlls/objects/425_BombPlant.h"
#include "dlls/objects/426_BombPlantSp.h"
#include "dlls/objects/427_BombPlantin.h"
#include "dlls/objects/428_SH_queenear.h"
#include "dlls/objects/429_SH_thorntai.h"
#include "dlls/objects/430_SH_LevelCon.h"
#include "dlls/objects/431_SH_swaplift.h"
#include "dlls/objects/432_SH_swapston.h"
#include "dlls/objects/433_SH_staff.h"
#include "dlls/objects/434_SH_staffHaz.h"
#include "dlls/objects/435_SH_Beacon.h"
#include "dlls/objects/436_SH_EmptyTum.h"
#include "dlls/objects/437.h"
#include "dlls/objects/438_SC_levelcon.h"
#include "dlls/objects/439.h"
#include "dlls/objects/440_SC_totempol.h"
#include "dlls/objects/441_SC_Cloudrun.h"
#include "dlls/objects/442_SC_totempuz.h"
#include "dlls/objects/443_SC_totembon.h"
#include "dlls/objects/444_SC_totemstr.h"
#include "dlls/objects/445.h"
#include "dlls/objects/446.h"
#include "dlls/objects/447_DIMLavaBall.h"
#include "dlls/objects/448_DIMLogFire.h"
#include "dlls/objects/449_DIMSnowBall.h"
#include "dlls/objects/450_DIMSnowBall.h"
#include "dlls/objects/451_DIMGate.h"
#include "dlls/objects/452_DIMIceWall.h"
#include "dlls/objects/453_DIMBarrier.h"
#include "dlls/objects/454_DIMCannon.h"
#include "dlls/objects/455_DIMLavaSmas.h"
#include "dlls/objects/456_DIMBridgeCo.h"
#include "dlls/objects/457_DIMDismount.h"
#include "dlls/objects/458_DIMExplosio.h"
#include "dlls/objects/459_DIMWoodDoor.h"
#include "dlls/objects/460_DIMMagicBri.h"
#include "dlls/objects/461_DIM_LevelCo.h"
#include "dlls/objects/462.h"
#include "dlls/objects/463.h"
#include "dlls/objects/465_DIMTruthHor.h"
#include "dlls/objects/466_WORLDplanet.h"
#include "dlls/objects/467.h"
#include "dlls/objects/468_WORLDAstero.h"
#include "dlls/objects/469_DIM2Conveyo.h"
#include "dlls/objects/470.h"
#include "dlls/objects/471_DIM2SnowBal.h"
#include "dlls/objects/472_DIM2PathGen.h"
#include "dlls/objects/473_DIM2PrisonM.h"
#include "dlls/objects/474.h"
#include "dlls/objects/475.h"
#include "dlls/objects/476_DIM2IceFloe.h"
#include "dlls/objects/477_DIM2Icicle.h"
#include "dlls/objects/478_DIM2LavaCon.h"
#include "dlls/objects/479.h"
#include "dlls/objects/480_DIM_Boss.h"
#include "dlls/objects/481_DIM_BossGut.h"
#include "dlls/objects/482_DIM_BossTon.h"
#include "dlls/objects/483_DIM_BossGut.h"
#include "dlls/objects/484_MAGICMaker.h"
#include "dlls/objects/485_DIM_BossSpi.h"
#include "dlls/objects/486_DIMbosscrac.h"
#include "dlls/objects/487_DIMbossfire.h"
#include "dlls/objects/488_SB_Galleon.h"
#include "dlls/objects/489_SB_Propelle.h"
#include "dlls/objects/490_SB_ShipHead.h"
#include "dlls/objects/491_SB_ShipMast.h"
#include "dlls/objects/492_SB_ShipGun.h"
#include "dlls/objects/493_SB_FireBall.h"
#include "dlls/objects/494_SB_CannonBa.h"
#include "dlls/objects/495_SB_CloudBal.h"
#include "dlls/objects/496_SB_KyteCage.h"
#include "dlls/objects/497_SB_SeqDoor.h"
#include "dlls/objects/498_SB_CageKyte.h"
#include "dlls/objects/499_SB_MiniFire.h"
#include "dlls/objects/500.h"
#include "dlls/objects/501.h"
#include "dlls/objects/502.h"
#include "dlls/objects/503_SB_ShipGunB.h"
#include "dlls/objects/504_WM_Galleon.h"
#include "dlls/objects/505_WM_ObjCreat.h"
#include "dlls/objects/506_WM_seqobjec.h"
#include "dlls/objects/507.h"
#include "dlls/objects/508.h"
#include "dlls/objects/509_WM_LaserTar.h"
#include "dlls/objects/510.h"
#include "dlls/objects/511.h"
#include "dlls/objects/512.h"
#include "dlls/objects/513_WM_colrise.h"
#include "dlls/objects/516_WM_Torch.h"
#include "dlls/objects/518_LightSource.h"
#include "dlls/objects/519_WM_Worm.h"
#include "dlls/objects/521_WM_LevelCon.h"
#include "dlls/objects/522_WM_GeneralS.h"
#include "dlls/objects/599_DR_EarthWar.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/audio/sfx.h"
#include "main/dll/CAM/dll_0001_camcontrol.h"
#include "main/dll/dll_0042_cameramodenormal.h"
#include "main/dll/dll_0043_cameramodestaffanim.h"
#include "main/dll/dll_0044_cameramodeviewfinder.h"
#include "main/dll/CAM/dll_0045_camTalk.h"
#include "main/dll/dll_0046_cameramodedebug.h"
#include "main/dll/dll_0047_cameramodepath.h"
#include "main/dll/dll_0048_cameramodestatic.h"
#include "main/dll/dll_0049_cameramodecombat.h"
#include "main/dll/dll_004A_cameramodeshipbattle.h"
#include "main/dll/dll_004B_cameramodeclimb.h"
#include "main/dll/dll_004C_cameramodefixed.h"
#include "main/dll/dll_004D_cameramodenpcspeak.h"
#include "main/dll/dll_004E_cameramodeworldmap.h"
#include "main/dll/dll_004F_cameramode.h"
#include "main/dll/dll_0050_cameramodecrawl.h"
#include "main/dll/dll_0051_cameramodecannon.h"
#include "main/dll/dll_0052_cameramodeforcebehind.h"
#include "main/dll/dll_0053_cameramodecloudrunner.h"
#include "main/dll/dll_0054_dll54.h"
#include "main/dll/dll_0055_cameramode.h"
#include "main/dll/dll_0056_cameramodearwing.h"
#include "main/dll/dll_0057_cameramodetitle.h"
#include "main/dll/dll_0058_dummy58.h"
#include "main/dll/dll_0059_dll59func0.h"
#include "main/dll/dll_005A_staffcollision.h"
#include "main/dll/dll_005B_modgfx.h"
#include "main/dll/dll_005C_modgfx.h"
#include "main/dll/dll_005D_modgfx.h"
#include "main/dll/dll_005E_modgfx.h"
#include "main/dll/dll_005F_modgfx.h"
#include "main/dll/dll_0060_modgfx.h"
#include "main/dll/dll_0061_modgfx.h"
#include "main/dll/dll_0062_modgfx.h"
#include "main/dll/dll_0063_modgfx.h"
#include "main/dll/dll_0064_modgfx.h"
#include "main/dll/dll_0065_modgfx.h"
#include "main/dll/dll_0066_modgfx.h"
#include "main/dll/dll_0067_modgfx.h"
#include "main/dll/dll_0068_modgfx.h"
#include "main/dll/dll_0069_modgfx.h"
#include "main/dll/dll_006A_modgfx.h"
#include "main/dll/dll_006B_modgfx.h"
#include "main/dll/dll_006C_dummy6c.h"
#include "main/dll/dll_006D_modgfx.h"
#include "main/dll/dll_006E_modgfx.h"
#include "main/dll/dll_006F_modgfx.h"
#include "main/dll/dll_0070_modgfx.h"
#include "main/dll/dll_0071_modgfx.h"
#include "main/dll/dll_0072_modgfx.h"
#include "main/dll/dll_0073_modgfx.h"
#include "main/dll/dll_0074_modgfx.h"
#include "main/dll/dll_0075_modgfx.h"
#include "main/dll/dll_0076_modgfx.h"
#include "main/dll/dll_0077_modgfx.h"
#include "main/dll/dll_0078_modgfx.h"
#include "main/dll/dll_0079_modgfx.h"
#include "main/dll/dll_007A_modgfx.h"
#include "main/dll/dll_007B_modgfx.h"
#include "main/dll/dll_007C_modgfx.h"
#include "main/dll/dll_007D_modgfx.h"
#include "main/dll/dll_007E_modgfx.h"
#include "main/dll/dll_007F_modgfx.h"
#include "main/dll/dll_0080_modgfx.h"
#include "main/dll/dll_0081_modgfx.h"
#include "main/dll/dll_0082_modgfx.h"
#include "main/dll/dll_0083_modgfx.h"
#include "main/dll/dll_0084_modgfx.h"
#include "main/dll/dll_0085_modgfx.h"
#include "main/dll/dll_0086_modgfx.h"
#include "main/dll/dll_0087_modgfx.h"
#include "main/dll/dll_0088_modgfx.h"
#include "main/dll/dll_0089_modgfx.h"
#include "main/dll/dll_008A_modgfx.h"
#include "main/dll/dll_008B_modgfx.h"
#include "main/dll/dll_008C_modgfx.h"
#include "main/dll/dll_008D_modgfx.h"
#include "main/dll/dll_008E_modgfx.h"
#include "main/dll/dll_008F_modgfx.h"
#include "main/dll/dll_0090_modgfx.h"
#include "main/dll/dll_0091_modgfx.h"
#include "main/dll/dll_0092_modgfx.h"
#include "main/dll/dll_0093_modgfx.h"
#include "main/dll/dll_0094_modgfx.h"
#include "main/dll/dll_0095_modgfx.h"
#include "main/dll/dll_0096_modgfx.h"
#include "main/dll/dll_0097_modgfx.h"
#include "main/dll/dll_0098_modgfx.h"
#include "main/dll/dll_0099_modgfx.h"
#include "main/dll/dll_009A_modgfx.h"
#include "main/dll/dll_009B_modgfx.h"
#include "main/dll/dll_009C_modgfx.h"
#include "main/dll/dll_009D_modgfx.h"
#include "main/dll/dll_009E_modgfx.h"
#include "main/dll/dll_009F_modgfx.h"
#include "main/dll/dll_00A0_modgfx.h"
#include "main/dll/dll_00A1_modgfx.h"
#include "main/dll/dll_00A2_modgfx.h"
#include "main/dll/dll_00A3_modgfx.h"
#include "main/dll/dll_00A4_dummya4.h"
#include "main/dll/dll_00A5_modgfx.h"
#include "main/dll/dll_00A6_modgfx.h"
#include "main/dll/dll_00A7_modgfx.h"
#include "main/dll/dll_00A8_modgfx.h"
#include "main/dll/dll_00A9_modgfx.h"
#include "main/dll/dll_00AA_modgfx.h"
#include "main/dll/dll_00AB_projdummy.h"
#include "main/dll/dll_00AC_projmagicstream.h"
#include "main/dll/dll_00AD_projmagicemmit1.h"
#include "main/dll/dll_00AE_projroombeam.h"
#include "main/dll/dll_00AF_projlightning1.h"
#include "main/dll/dll_00B0_projlightning2.h"
#include "main/dll/dll_00B1_projlightning3.h"
#include "main/dll/dll_00B2_projrobotfire.h"
#include "main/dll/dll_00B3_projlightning4.h"
#include "main/dll/dll_00B4_projenergise1.h"
#include "main/dll/dll_00B5_projenergise2.h"
#include "main/dll/dll_00B6_projsquirt1.h"
#include "main/dll/dll_00B7_projship1.h"
#include "main/dll/dll_00B8_projlightning5.h"
#include "main/dll/dll_00B9_projlightning7.h"
#include "main/dll/dll_00BA_projlightning6.h"
#include "main/dll/dll_00BB_projwallpower.h"
#include "main/dll/dll_00BC_projquakeshock.h"
#include "main/dll/dll_00BD_projsunshock.h"
#include "main/dll/dll_00BE_projtesla.h"
#include "main/dll/dll_00BF_projcore1.h"
#include "main/dll/dll_00C0_projcore2.h"
#include "main/dll/dll_00C1_projcore3.h"
#include "main/dll/dll_00C2_projdfp1r.h"
#include "main/dll/dll_0000_gameui.h"
#include "main/dll/dll_00DA_pollenfragment.h"
#include "main/dll/dll_0282_barrelgener.h"
#include "main/dll/dll_0293_suntemple.h"
#include "main/dll/dll_0294_wctemple.h"
#include "main/dll/WC/dll_0292_wctrexstatu.h"
#include "main/dll/WC/dll_028F_wcpressures.h"
#include "main/dll/WC/dll_0295_wcapertures.h"
#include "main/dll/WC/dll_0296_wctempledia.h"
#include "main/dll/WC/dll_0298_wcfloortile.h"
#include "main/dll/WC/WCbeacon.h"
#include "main/dll/ARW/dll_029C_arwarwingbo.h"
#include "main/dll/ARW/dll_029D_arwarwinggu.h"
#include "main/dll/dll_029B_arwingandrossstuff.h"
#include "main/dll/ARW/dll_02A1_arwlevelcon.h"
#include "main/dll/ARW/dll_029A_arwarwing.h"
#include "main/dll/DR/dll_0281_drearthcal.h"
#include "main/dll/dll_0299.h"
#include "main/dll/dll_02B1_cmbsrc.h"
#include "main/dll/dll_02B2_dustmotesou.h"
#include "main/dll/dll_02B4_cntcounter.h"
#include "main/dll/dll_02B6_cnthitobjec.h"
#include "main/dll/dll_02B7_mcupgrade.h"
#include "main/dll/dll_02B8_mcupgradema.h"
#include "main/dll/dll_02B9_mcstaffeffe.h"
#include "main/dll/dll_02BA_mclightning.h"
#include "main/dll/dll_02BB_gflevelcon.h"
#include "main/dll/dll_02BC_andross.h"
#include "main/dll/dll_02BF_androssligh.h"
#include "main/dll/dll_02BE_androssbrain.h"
#include "main/dll/dll_02BD_androsshand.h"
#include "main/dll/dll_02AF_tree.h"
#include "main/dll/dll_02B0_brokenpipe.h"
#include "main/frame_timing.h"
#include "main/game_timer_control.h"
#include "main/gametext_box.h"
#include "main/gametext_show_str.h"
#include "main/gametext_color.h"
#include "main/minimap.h"
#include "main/model_engine.h"
#include "main/mm.h"
#include "main/pause_menu.h"
#include "main/resource.h"
#include "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/printf.h"
#include "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/string.h"
#include "main/hud_visibility.h"

s32 gModelEngineHudNumber = -1;
f32 gModelEngineTimerValue;
f32 gModelEngineTimerDuration;
s8 gModelEngineTimerFlags;
u8 gModelEngineTimerState;
int gModelEnginePrevUiDll;
int curUiDll;
int gModelEnginePendingUiDll;
UiDllVTable** gModelEngineCurUiDllRes;
int gModelEngineTimerDigitPairXOffset = 0x10;
int gModelEngineTimerFieldXStride = 0x26;
int gModelEngineTimerColonX = 0x24;
int gModelEngineTimerDotX = 0x4A;
s32 gMenuState = -1;
char sModelEngineHudNumberFormat[] = "%d";
char sModelEngineTimerDigitFormat[] = "%01d";
char sModelEngineTimerColonText[] = ":";
char sModelEngineTimerDotText[] = ".";

#define RESOURCE_DESCRIPTOR_COUNT 0x2c1

/* gModelEngineTimerState bits (roles from accessor fns: timerSetToCountUp,
 * isGameTimerDisabled, gameTimerIsRunning). */
#define MODELENGINE_TIMER_COUNTDOWN 1
#define MODELENGINE_TIMER_DISABLED  2
#define MODELENGINE_TIMER_RUNNING   4

extern ResourceDescriptor Carryable_funcs, boneParticleEffect_funcs, dll_19;
extern ResourceDescriptor dll_2E, expgfx_funcs;
extern ResourceDescriptor gARWBlockerObjDescriptor, gARWBombCollObjDescriptor, gARWGeneratoObjDescriptor,
    gARWProximitObjDescriptor;
extern ResourceDescriptor gARWSpeedStrObjDescriptor, gARWSquadronObjDescriptor;
extern ResourceDescriptor gBaddieObjDescriptor, gBossDrakorObjDescriptor;
extern ResourceDescriptor gChukaObjDescriptor;
extern ResourceDescriptor gControlLightObjDescriptor, gCrCloudRaceObjDescriptor, gCrFuelTankObjDescriptor;
extern ResourceDescriptor gDBHoleControl1ObjDescriptor, gDB_eggObjDescriptor, gDBstealerwormObjDescriptor,
    gDFP_LevelControlObjDescriptor, gDFP_ObjCreatorObjDescriptor, gDFP_TorchObjDescriptor;
extern ResourceDescriptor gDFP_seqpointObjDescriptor;
extern ResourceDescriptor gDIMSnowHorn1ObjDescriptor;
extern ResourceDescriptor gDIM_trickyObjDescriptor, gDR_CloudRunnerObjDescriptor;
extern ResourceDescriptor gDfperchwitchObjDescriptor, gDfpfloorbarObjDescriptor, gDfplightniObjDescriptor,
    gDfppowerslObjDescriptor;
extern ResourceDescriptor gDfpstatue1ObjDescriptor, gDfptargetblockObjDescriptor, gDirectionalLightObjDescriptor;
extern ResourceDescriptor gDoorswitchObjDescriptor, gDrBarrelGrObjDescriptor, gDrCageControlObjDescriptor,
    gDrCageWithObjDescriptor, gDrChimmeyObjDescriptor, gDrCloudPerObjDescriptor, gDrCreatorObjDescriptor;
extern ResourceDescriptor gDrEnergyDiscObjDescriptor, gDrGeneratorObjDescriptor, gDrLaserCannonObjDescriptor,
    gDrLightBeaObjDescriptor, gDrMusicContObjDescriptor, gDrShackleObjDescriptor, gDrakorDThornBushObjDescriptor,
    gDrakorEnergyObjDescriptor;
extern ResourceDescriptor gDrakorHoverPadObjDescriptor, gDrakorMissileObjDescriptor;
extern ResourceDescriptor gEarthWalkerObjDescriptor, gExplodePlanObjDescriptor;
extern ResourceDescriptor gFireFlyObjDescriptor, gFireObjDescriptor;
extern ResourceDescriptor gFirePipeObjDescriptor;
extern ResourceDescriptor gGmMazeWellObjDescriptor;
extern ResourceDescriptor gHighTopObjDescriptor;
extern ResourceDescriptor gKtFallingrocksObjDescriptor;
extern ResourceDescriptor gKtLazerlightObjDescriptor, gKtLazerwallObjDescriptor, gKtRexFloorSwitchObjDescriptor,
    gKtRexLevelObjDescriptor, gKtRexObjDescriptor, gKytesMumObjDescriptor;
extern ResourceDescriptor gDfpSpellPlaceObjDescriptor, gLaserUnsupportedObjDescriptor;
extern ResourceDescriptor gMoonSeedPlantingSpotObjDescriptor;
extern ResourceDescriptor gPlatform1ObjDescriptor, gPointLightObjDescriptor;
extern ResourceDescriptor gProjectedLightObjDescriptor, gProximityMineObjDescriptor;
extern ResourceDescriptor gRingObjDescriptor, gSB_CloudRunnerObjDescriptor;
extern ResourceDescriptor gSPDrapeObjDescriptor, gSPScarabObjDescriptor, gSPitembeamObjDescriptor,
    gSeqPointObjDescriptor;
extern ResourceDescriptor gDFP_RotatePObjDescriptor, gShopItemObjDescriptor, gShopKeeperObjDescriptor,
    gShopObjDescriptor;
extern ResourceDescriptor gSnowBikeObjDescriptor, gSnowClawObjDescriptor;
extern ResourceDescriptor gSoftBodyObjDescriptor, gSpellStoneObjDescriptor, gStaffObjDescriptor,
    gStaticCameraObjDescriptor;
extern ResourceDescriptor gTextBlockObjDescriptor, gTimerObjDescriptor;
extern ResourceDescriptor gTitleScreenObjDescriptor, gTrickyCurveObjDescriptor;
extern ResourceDescriptor gTrickyObjDescriptor, gTriggerObjDescriptor, gVFPDragHeadObjDescriptor, gVFPLiftObjDescriptor,
    gVFP_Block1ObjDescriptor;
extern ResourceDescriptor gVFP_DoorSwitchObjDescriptor, gVFP_LaddersObjDescriptor, gVFP_LevelControlObjDescriptor,
    gVFP_MiniFireObjDescriptor, gVFP_ObjCreatorObjDescriptor, gVFP_PlatformObjDescriptor, gVfpSpellPlaceObjDescriptor,
    gVFP_coreplatObjDescriptor, gVFP_flamepointObjDescriptor;
extern ResourceDescriptor gVFP_lavapoolObjDescriptor, gVFP_lavastarObjDescriptor, gVFP_statueballObjDescriptor,
    gVortexObjDescriptor, gWCBouncyCraObjDescriptor;
extern ResourceDescriptor gWCLevelContObjDescriptor, gWCPushBlockObjDescriptor, gWCTempleBriObjDescriptor,
    gWCTileObjDescriptor;
extern ResourceDescriptor gWM_PlanetsObjDescriptor;
extern ResourceDescriptor gWM_SpiritSetObjDescriptor, gWM_WallCrawlerObjDescriptor, gWM_newcrystalObjDescriptor,
    gWM_seqpointObjDescriptor;
extern ResourceDescriptor gWM_spiritplaceObjDescriptor, gWM_sunObjDescriptor, gWaterFlowWeObjDescriptor;
extern ResourceDescriptor ObjSeq_funcs;
extern ResourceDescriptor sky_funcs, sky2_funcs, newclouds_funcs, Dummy08_funcs, cloudaction_funcs, waterfx_funcs,
    dll_0B_funcs, partfx_funcs;
extern ResourceDescriptor Effect1_funcs, Effect2_funcs, Effect3_funcs, Effect4_funcs, Effect5_funcs, Effect6_funcs,
    Effect7_funcs, Effect8_funcs;
extern ResourceDescriptor Effect9_funcs, Effect10_funcs, Effect11_funcs, Effect12_funcs, Effect14_funcs, Effect16_funcs,
    Effect15_funcs, Effect13_funcs;
extern ResourceDescriptor Effect17_funcs, Effect18_funcs, Effect19_funcs, Effect20_funcs, Checkpoint_funcs,
    screenTransition_funcs, Dummy04_funcs, player_funcs;
extern ResourceDescriptor UIController_funcs, Dummy12_funcs, RomCurve_funcs, dll_15_funcs, SaveGame_funcs,
    screens_funcs;
extern ResourceDescriptor Dummy30_funcs;
extern ResourceDescriptor TitleScreenInit_funcs, n_rareware_funcs, n_attractmode_funcs, SaveSelectScreen_funcs,
    EnterSaveNameScreen_funcs, OptionsScreen_funcs, WeirdUnusedMenu_funcs, Dummy39_funcs;
extern ResourceDescriptor Dummy3A_funcs, GameUI_funcs, Menu_funcs, Link_funcs, TitleMenuItem_funcs, Dummy3E_funcs,
    Minimap_funcs, dll_3F_funcs;
extern ResourceDescriptor gCreditsDescriptor, gWarpStoneUiDescriptor;
extern ResourceDescriptor gGCRobotBlastObjDescriptor, gDll22CObjDescriptor, gDll29EObjDescriptor;
extern ResourceDescriptor playerShadow_funcs, projgfx_funcs;
extern ObjectDescriptor gPlayerObjDescriptor, gDllC5ObjDescriptor, gDllD3ObjDescriptor, gDllF7ObjDescriptor;
extern ObjectDescriptor gDll146ObjDescriptor, gDll147ObjDescriptor, gDll14DObjDescriptor, gDll151ObjDescriptor;
extern ObjectDescriptor gDll152ObjDescriptor, gDll155ObjDescriptor, gDll156ObjDescriptor, gDll15CObjDescriptor;
extern ObjectDescriptor gDll15EObjDescriptor, gDll160ObjDescriptor, gDll161ObjDescriptor, gDll163ObjDescriptor;
extern ObjectDescriptor gDll165ObjDescriptor, gDll168ObjDescriptor, gDll176ObjDescriptor, gDll17CObjDescriptor;
extern ObjectDescriptor gDll202ObjDescriptor, gDll203ObjDescriptor, gDll205ObjDescriptor, gDll208ObjDescriptor;
extern ObjectDescriptor gDll212ObjDescriptor, gDll213ObjDescriptor, gDll214ObjDescriptor, gDll219ObjDescriptor;
extern ObjectDescriptor gDll21BObjDescriptor, gSpellStoneUseObjDescriptor, gDll23DObjDescriptor, gDll23EObjDescriptor;
extern ObjectDescriptor gDll244ObjDescriptor, gDll245ObjDescriptor, gDll246ObjDescriptor, gDll247ObjDescriptor;
extern ObjectDescriptor gDll248ObjDescriptor, gDll249ObjDescriptor, gDll24AObjDescriptor, gDll24BObjDescriptor;
extern ObjectDescriptor gDll24CObjDescriptor, gDll264ObjDescriptor, gDll267ObjDescriptor, gDll26AObjDescriptor;
extern ObjectDescriptor gDll26DObjDescriptor, gDll270ObjDescriptor, gDll274ObjDescriptor, gDll275ObjDescriptor;
extern ObjectDescriptor gDll276ObjDescriptor, gDll277ObjDescriptor, gDll278ObjDescriptor, gDll27AObjDescriptor;
extern ObjectDescriptor gDll27BObjDescriptor, gDll27DObjDescriptor, gDll27FObjDescriptor, gDll28BObjDescriptor;
extern ObjectDescriptor gDll299ObjDescriptor, gDll2A3ObjDescriptor, gDll2A4ObjDescriptor;

void* gResourceLoadedHandles[0x2C1];
u16 gResourceRefCounts[0x2C2];
char gModelEngineTextBuf[0x10];

RingBufferQueue* Queue_Alloc(int capacity, int elemSize) {
    RingBufferQueue* queue = mmAlloc(elemSize * capacity + sizeof(RingBufferQueue), 0x1a, 0);
    queue->data = (u8*)queue + sizeof(RingBufferQueue);
    queue->count = 0;
    queue->capacity = capacity;
    queue->elemSize = elemSize;
    queue->writeIndex = 0;
    return queue;
}

s32 modelRenderInstrsState_getBit(ModelRenderInstrsState* state) {
    return state->bit;
}

void modelRenderInstrsState_setBit(ModelRenderInstrsState* state, s32 bit) {
    state->bit = bit;
}

void modelRenderInstrsState_init(ModelRenderInstrsState* state, void* instrs, int bitCount, int fieldC) {
    state->byteCount = bitCount >> 3;
    if ((bitCount & 7) != 0) {
        state->byteCount++;
    }
    state->bitCount = bitCount;
    state->fieldC = fieldC;
    state->instrs = instrs;
    state->bit = 0;
}

void objList_remove(ObjLinkedList* list, uintptr_t item) {
    uintptr_t head = list->head;
    if (head == item) {
        list->head = *(uintptr_t*)(head + list->nextOffset);
        list->count--;
        return;
    }

    uintptr_t current = head;
    uintptr_t prev = head;
    while (current != 0 && current != item) {
        prev = current;
        current = *(uintptr_t*)(current + list->nextOffset);
    }

    if (current == 0) {
        return;
    }

    uintptr_t next = *(uintptr_t*)(current + list->nextOffset);
    if (current == head) {
        list->head = next;
    } else {
        *(uintptr_t*)(prev + list->nextOffset) = next;
    }
    list->count--;
}

void objListAdd(ObjLinkedList* list, uintptr_t prev, uintptr_t item) {

    if (list->head == 0) {
        list->head = item;
    } else {
        uintptr_t next;
        if (prev == 0) {
            next = list->head;
            list->head = item;
        } else {
            next = *(uintptr_t*)(prev + list->nextOffset);
            *(uintptr_t*)(prev + list->nextOffset) = item;
        }
        *(uintptr_t*)(item + list->nextOffset) = next;
    }
    list->count++;
}

void objListInit(ObjLinkedList* list, s16 nextOffset) {
    list->head = 0;
    list->nextOffset = nextOffset;
}

BOOL model_findIdxInModelList(ModelList* list, void* header, int* outIndex) {
    s16* entry = list->entries;
    while (entry < list->end) {
        if (memcmp(entry + 1, header, list->dataSize) == 0) {
            *outIndex = *entry;
            return TRUE;
        }
        entry += list->strideShorts;
    }
    return FALSE;
}

BOOL ModelList_getHeader(ModelList* list, int index, void* outHeader) {
    s16* entry = list->entries;
    while (entry < list->end) {
        if (*entry == index) {
            memcpy(outHeader, entry + 1, list->dataSize);
            return TRUE;
        }
        entry += list->strideShorts;
    }
    return FALSE;
}

void model_adjustModelList(ModelList* list, int index) {
    s16* entry = list->entries;
    while (entry < list->end) {
        if (*entry == index) {
            *entry = -1;
            break;
        }
        entry += list->strideShorts;
    }

    while (list->end > list->entries && list->end[-1] == -1) {
        list->end -= list->strideShorts;
    }
}

void modelInitModelList(ModelList* list, s16 index, void* header) {
    s16* entry;

    for (entry = list->entries; entry < list->end; entry += list->strideShorts) {
        if (*entry == -1) {
            break;
        }
    }

    *entry = index;
    memcpy(entry + 1, header, list->dataSize);
    if (entry == list->end) {
        list->end += list->strideShorts;
    }
}

ModelList* allocModelStruct(int capacity, int dataSize) {
    int entryBytes = dataSize + 2;
    ModelList* list = mmAlloc(capacity * entryBytes + sizeof(ModelList), 0x1a, 0);
    list->entries = (s16*)((u8*)list + sizeof(ModelList));
    list->dataSize = dataSize;
    list->strideShorts = (u32)entryBytes >> 1;
    list->end = list->entries;
    list->capacityEnd = list->entries + capacity * list->strideShorts;
    memset(list->entries, -1, capacity * (list->strideShorts * 2));
    return list;
}

BOOL Resource_Release(void* handleSlot) {
    s32 i = 0;
    ResourceDescriptor* descriptor = handleSlot;
    while (i < RESOURCE_DESCRIPTOR_COUNT) {
        if ((void*)&gResourceLoadedHandles[i] == handleSlot) {
            descriptor = gResourceDescriptors[i];
            break;
        }
        i++;
    }

    gResourceRefCounts[i]--;
    if (gResourceRefCounts[i] == 0) {
        if (descriptor->release != NULL) {
            descriptor->release();
        }
        return TRUE;
    }
    return FALSE;
}

void* Resource_Acquire(u16 id, int unused) {
    ResourceDescriptor* descriptor = gResourceDescriptors[id];
    if (gResourceRefCounts[id] == 0 && descriptor->acquire != NULL) {
        descriptor->acquire(descriptor);
    }
    gResourceRefCounts[id]++;
    gResourceLoadedHandles[id] = descriptor->data;
    return &gResourceLoadedHandles[id];
}

void Resource_ResetRefCounts(void) {
    for (u32 i = 0; i < RESOURCE_DESCRIPTOR_COUNT; i++) {
        gResourceRefCounts[i] = 0;
    }
}

void menuSetState(s32 value) {
    gMenuState = value;
}

u8 gameTimerIsRunning(void) {
    return gModelEngineTimerState & MODELENGINE_TIMER_RUNNING;
}

void hudNumberRender(void* context) {
    if (gModelEngineHudNumber != -1) {
        sprintf(gModelEngineTextBuf, sModelEngineHudNumberFormat, gModelEngineHudNumber);
        gameTextShowStr(gModelEngineTextBuf, 13, 0, 0);
    }
}

void hudNumberSet(s32 value) {
    gModelEngineHudNumber = value;
}

void gameTimerRun(void* context) {
    f32 dt = timeDelta;
    u8 colorFlag = 0;
    TextSlot* box = gameTextGetBox(0xD);
    u16 boxY;

    if ((gModelEngineTimerState & MODELENGINE_TIMER_COUNTDOWN) || getHudHiddenFrameCount() != 0) {
        dt = 0.0f;
    }

    char clamped = 0;
    if ((gModelEngineTimerFlags & 1) != 0) {
        gModelEngineTimerValue -= dt;
        if (gModelEngineTimerValue <= 0.0f) {
            clamped = 1;
            gModelEngineTimerValue = 0.0f;
        }
        if (gModelEngineTimerValue < 600.0f) {
            colorFlag = 1;
        }
    } else {
        gModelEngineTimerValue += dt;
        if (gModelEngineTimerValue > gModelEngineTimerDuration) {
            clamped = 1;
            gModelEngineTimerValue = gModelEngineTimerDuration;
        }
        if (gModelEngineTimerValue > gModelEngineTimerDuration - 600.0f) {
            colorFlag = 1;
        }
    }

    if (clamped) {
        if ((gModelEngineTimerFlags & 8) != 0) {
            Sfx_PlayFromObject(0, SFXTRIG_sc_lockon22);
        }
        gModelEngineTimerState &= ~MODELENGINE_TIMER_RUNNING;
        gModelEngineTimerState |= MODELENGINE_TIMER_DISABLED;
    }

    if ((gModelEngineTimerFlags & 4) != 0) {
        f32 panByte;
        f32 volume;
        if (fhConfigRevision() == 0 || dt != 0.0f) {
            Sfx_KeepAliveLoopedObjectSound(0, SFXTRIG_sc_commsbleep_28c);
        }
        if ((gModelEngineTimerFlags & 1) != 0) {
            panByte = (f32)(0x7F - ((int)(80.0f * (gModelEngineTimerValue / gModelEngineTimerDuration)) & 0xFF));
            volume = 1.3f - 0.6f * (gModelEngineTimerValue / gModelEngineTimerDuration);
        } else {
            panByte = (f32)(((int)(80.0f * (gModelEngineTimerValue / gModelEngineTimerDuration)) & 0xFF) + 0x2F);
            volume = 0.6f * (gModelEngineTimerValue / gModelEngineTimerDuration) + 0.7f;
        }
        Sfx_SetObjectSfxVolume(0, SFXTRIG_sc_commsbleep_28c, panByte, volume);
    }

    if ((gModelEngineTimerFlags & 0x10) != 0 && pauseMenuState == 0 && getHudHiddenFrameCount() == 0) {
        int totalSecs = gModelEngineTimerValue;
        int mins = totalSecs / 60;
        int hours = mins / 60;
        int minutes = mins - hours * 60;
        int hundredths = (int)(100.0f * (gModelEngineTimerValue / 60.0f));
        hundredths = hundredths - hundredths / 100 * 100;

        boxY = getMinimapY() - 0x28;
        drawHudBox(0x32, (s16)(boxY - 4), 0x78, 0x28, 0xFF, 1);
        box->y = boxY;

        if (colorFlag && hundredths < 0x32) {
            gameTextSetColor(0xFF, 0x40, 0x40, 0xFF);
        } else {
            gameTextSetColor(0xFF, 0xFF, 0xFF, 0xFF);
        }

        sprintf(gModelEngineTextBuf, sModelEngineTimerDigitFormat, hours / 10);
        gameTextShowStr(gModelEngineTextBuf, 0xD, 5, 3);
        sprintf(gModelEngineTextBuf, sModelEngineTimerDigitFormat, hours % 10);
        gameTextShowStr(gModelEngineTextBuf, 0xD, gModelEngineTimerDigitPairXOffset + 5, 3);
        sprintf(gModelEngineTextBuf, sModelEngineTimerDigitFormat, minutes / 10);
        gameTextShowStr(gModelEngineTextBuf, 0xD, gModelEngineTimerFieldXStride + 5, 3);
        sprintf(gModelEngineTextBuf, sModelEngineTimerDigitFormat, minutes % 10);
        gameTextShowStr(gModelEngineTextBuf, 0xD, 5 + gModelEngineTimerFieldXStride + gModelEngineTimerDigitPairXOffset,
                        3);
        sprintf(gModelEngineTextBuf, sModelEngineTimerDigitFormat, hundredths / 10);
        gameTextShowStr(gModelEngineTextBuf, 0xD, gModelEngineTimerFieldXStride * 2 + 5, 3);
        sprintf(gModelEngineTextBuf, sModelEngineTimerDigitFormat, hundredths % 10);
        gameTextShowStr(gModelEngineTextBuf, 0xD,
                        5 + gModelEngineTimerFieldXStride * 2 + gModelEngineTimerDigitPairXOffset, 3);
        if (minutes & 1) {
            gameTextShowStr(sModelEngineTimerColonText, 0xD, gModelEngineTimerColonX, 3);
            gameTextShowStr(sModelEngineTimerDotText, 0xD, gModelEngineTimerDotX, 3);
        }
    }
}

f32 gameTimerGetElapsedMilliseconds(void) {
    if ((gModelEngineTimerFlags & 1) != 0) {
        return 1000.0f * ((gModelEngineTimerDuration - gModelEngineTimerValue) / 60.0f);
    }
    return 1000.0f * (gModelEngineTimerValue / 60.0f);
}

f32 gameTimerGetValue(void) {
    return gModelEngineTimerValue;
}

int isGameTimerDisabled(void) {
    return gModelEngineTimerState & MODELENGINE_TIMER_DISABLED;
}

void gameTimerStop(void) {
    gModelEngineTimerState &= ~MODELENGINE_TIMER_RUNNING;
    gModelEngineTimerState |= MODELENGINE_TIMER_DISABLED;
}

void timerSetToCountUp(void) {
    if ((gModelEngineTimerState & MODELENGINE_TIMER_COUNTDOWN) != 0) {
        gModelEngineTimerState &= ~MODELENGINE_TIMER_COUNTDOWN;
    }
}

void gameTimerInit(s8 flags, int minutes) {
    gModelEngineTimerFlags = flags;
    if ((flags & 1) != 0) {
        gModelEngineTimerValue = minutes * 60;
    } else {
        gModelEngineTimerValue = 0.0f;
    }
    gModelEngineTimerDuration = minutes * 60;
    gModelEngineTimerState |= MODELENGINE_TIMER_COUNTDOWN;
    gModelEngineTimerState &= ~MODELENGINE_TIMER_DISABLED;
    if ((flags & 3) != 0) {
        gModelEngineTimerState |= MODELENGINE_TIMER_RUNNING;
    } else {
        gModelEngineTimerState &= ~MODELENGINE_TIMER_RUNNING;
    }
}

void curUiDllDraw(int a, int b, int c, int d) {
    if (gModelEngineCurUiDllRes != NULL) {
        UiDllVTable* callbacks = *gModelEngineCurUiDllRes;
        callbacks->draw(a, b, c);
    }
}

void uiDll_runFrameEndAndLoadNext(void) {
    if (gModelEngineCurUiDllRes != NULL) {
        UiDllVTable* callbacks = *gModelEngineCurUiDllRes;
        callbacks->frameEnd();
    }

    if (gModelEnginePendingUiDll != 0) {
        gModelEnginePendingUiDll--;
        gModelEnginePrevUiDll = curUiDll;
        if (gModelEngineCurUiDllRes != NULL) {
            Resource_Release(gModelEngineCurUiDllRes);
            gModelEngineCurUiDllRes = NULL;
        }

        s32 resourceId = gModelEngineUiDllResourceIds[gModelEnginePendingUiDll];
        if (resourceId != -1) {
            gModelEngineCurUiDllRes = Resource_Acquire((u16)resourceId, 1);
        } else {
            gModelEngineCurUiDllRes = NULL;
            gModelEnginePendingUiDll = 0;
        }
        curUiDll = gModelEnginePendingUiDll;
        gModelEnginePendingUiDll = 0;
    }
}

int uiDll_runFrameStartAndLoadNext(void) {

    int result = 0;
    if (gModelEngineCurUiDllRes != NULL) {
        UiDllVTable* callbacks = *gModelEngineCurUiDllRes;
        result = callbacks->frameStart();
    }

    if (gModelEnginePendingUiDll != 0) {
        gModelEnginePendingUiDll--;
        gModelEnginePrevUiDll = curUiDll;
        if (gModelEngineCurUiDllRes != NULL) {
            Resource_Release(gModelEngineCurUiDllRes);
            gModelEngineCurUiDllRes = NULL;
        }

        s32 resourceId = gModelEngineUiDllResourceIds[gModelEnginePendingUiDll];
        if (resourceId != -1) {
            gModelEngineCurUiDllRes = Resource_Acquire((u16)resourceId, 1);
        } else {
            gModelEngineCurUiDllRes = NULL;
            gModelEnginePendingUiDll = 0;
        }
        curUiDll = gModelEnginePendingUiDll;
        gModelEnginePendingUiDll = 0;
    }
    return result;
}

void setCurUiDll(int idx) {
    curUiDll = idx;
}

int getPrevUiDll(void) {
    return gModelEnginePrevUiDll;
}

UiDllVTable** getCurUiDllInterface(void) {
    return gModelEngineCurUiDllRes;
}

int getCurUiDll(void) {
    return curUiDll;
}

void loadUiDll(int index) {
    s32 current = curUiDll;
    if (index == current) {
        return;
    }

    s32 next = index + 1;
    gModelEnginePendingUiDll = next;
    if (gModelEngineCurUiDllRes == NULL && next != 0) {
        gModelEnginePendingUiDll = next - 1;
        gModelEnginePrevUiDll = current;
        if (gModelEngineCurUiDllRes != NULL) {
            Resource_Release(gModelEngineCurUiDllRes);
            gModelEngineCurUiDllRes = NULL;
        }

        s32 resourceId = gModelEngineUiDllResourceIds[gModelEnginePendingUiDll];
        if (resourceId != -1) {
            gModelEngineCurUiDllRes = Resource_Acquire((u16)resourceId, 1);
        } else {
            gModelEngineCurUiDllRes = NULL;
            gModelEnginePendingUiDll = 0;
        }
        curUiDll = gModelEnginePendingUiDll;
        gModelEnginePendingUiDll = 0;
    }
}

void initGameTimer(void) {
    gModelEngineCurUiDllRes = NULL;
    gModelEnginePendingUiDll = 0;
    gModelEnginePrevUiDll = 0;
    curUiDll = 0;
    gModelEngineTimerState = MODELENGINE_TIMER_DISABLED;
    gModelEngineTimerFlags = 0;
    gModelEngineTimerValue = 0.0f;
    gModelEngineTimerDuration = 0.0f;
}

ResourceDescriptor* gResourceDescriptors[] = {
    RESOURCE_DESCRIPTOR_REF(GameUI_funcs),
    RESOURCE_DESCRIPTOR_REF(gCamcontrolResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(ObjSeq_funcs),
    RESOURCE_DESCRIPTOR_REF(Checkpoint_funcs),
    RESOURCE_DESCRIPTOR_REF(Dummy04_funcs),
    RESOURCE_DESCRIPTOR_REF(sky_funcs),
    RESOURCE_DESCRIPTOR_REF(sky2_funcs),
    RESOURCE_DESCRIPTOR_REF(newclouds_funcs),
    RESOURCE_DESCRIPTOR_REF(Dummy08_funcs),
    RESOURCE_DESCRIPTOR_REF(cloudaction_funcs),
    RESOURCE_DESCRIPTOR_REF(expgfx_funcs),
    RESOURCE_DESCRIPTOR_REF(dll_0B_funcs),
    RESOURCE_DESCRIPTOR_REF(projgfx_funcs),
    RESOURCE_DESCRIPTOR_REF(playerShadow_funcs),
    RESOURCE_DESCRIPTOR_REF(partfx_funcs),
    RESOURCE_DESCRIPTOR_REF(player_funcs),
    RESOURCE_DESCRIPTOR_REF(UIController_funcs),
    RESOURCE_DESCRIPTOR_REF(screens_funcs),
    RESOURCE_DESCRIPTOR_REF(Dummy12_funcs),
    RESOURCE_DESCRIPTOR_REF(waterfx_funcs),
    RESOURCE_DESCRIPTOR_REF(RomCurve_funcs),
    RESOURCE_DESCRIPTOR_REF(dll_15_funcs),
    RESOURCE_DESCRIPTOR_REF(screenTransition_funcs),
    RESOURCE_DESCRIPTOR_REF(SaveGame_funcs),
    RESOURCE_DESCRIPTOR_REF(boneParticleEffect_funcs),
    RESOURCE_DESCRIPTOR_REF(dll_19),
    RESOURCE_DESCRIPTOR_REF(Effect1_funcs),
    RESOURCE_DESCRIPTOR_REF(Effect2_funcs),
    RESOURCE_DESCRIPTOR_REF(Effect3_funcs),
    RESOURCE_DESCRIPTOR_REF(Effect4_funcs),
    RESOURCE_DESCRIPTOR_REF(Effect5_funcs),
    RESOURCE_DESCRIPTOR_REF(Effect6_funcs),
    RESOURCE_DESCRIPTOR_REF(Effect7_funcs),
    RESOURCE_DESCRIPTOR_REF(Effect8_funcs),
    RESOURCE_DESCRIPTOR_REF(Effect9_funcs),
    RESOURCE_DESCRIPTOR_REF(Effect10_funcs),
    RESOURCE_DESCRIPTOR_REF(Effect11_funcs),
    RESOURCE_DESCRIPTOR_REF(Effect12_funcs),
    RESOURCE_DESCRIPTOR_REF(Effect13_funcs),
    RESOURCE_DESCRIPTOR_REF(Effect14_funcs),
    RESOURCE_DESCRIPTOR_REF(Effect15_funcs),
    RESOURCE_DESCRIPTOR_REF(Effect16_funcs),
    RESOURCE_DESCRIPTOR_REF(Effect17_funcs),
    RESOURCE_DESCRIPTOR_REF(Effect18_funcs),
    RESOURCE_DESCRIPTOR_REF(Effect19_funcs),
    RESOURCE_DESCRIPTOR_REF(Effect20_funcs),
    RESOURCE_DESCRIPTOR_REF(dll_2E),
    RESOURCE_DESCRIPTOR_REF(Carryable_funcs),
    RESOURCE_DESCRIPTOR_REF(Dummy30_funcs),
    RESOURCE_DESCRIPTOR_REF(Minimap_funcs),
    RESOURCE_DESCRIPTOR_REF(TitleScreenInit_funcs),
    RESOURCE_DESCRIPTOR_REF(n_rareware_funcs),
    RESOURCE_DESCRIPTOR_REF(n_attractmode_funcs),
    RESOURCE_DESCRIPTOR_REF(SaveSelectScreen_funcs),
    RESOURCE_DESCRIPTOR_REF(EnterSaveNameScreen_funcs),
    RESOURCE_DESCRIPTOR_REF(OptionsScreen_funcs),
    RESOURCE_DESCRIPTOR_REF(WeirdUnusedMenu_funcs),
    RESOURCE_DESCRIPTOR_REF(Dummy39_funcs),
    RESOURCE_DESCRIPTOR_REF(Dummy3A_funcs),
    RESOURCE_DESCRIPTOR_REF(Menu_funcs),
    RESOURCE_DESCRIPTOR_REF(Link_funcs),
    RESOURCE_DESCRIPTOR_REF(TitleMenuItem_funcs),
    RESOURCE_DESCRIPTOR_REF(Dummy3E_funcs),
    RESOURCE_DESCRIPTOR_REF(dll_3F_funcs),
    RESOURCE_DESCRIPTOR_REF(gCreditsDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWarpStoneUiDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCameraModeNormalDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCameraModeStaffAnimDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCameraModeViewfinderDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCameraModeTalkDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCameraModeDebugDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCameraModePathDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCameraModeStaticDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCameraModeCombatDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCameraModeShipBattleDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCameraModeClimbDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCameraModeFixedDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCameraModeNpcSpeakDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCameraModeWorldMapDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCameraMode4FDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCameraModeCrawlDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCameraModeCannonDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCameraModeForceBehindDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCameraModeCloudRunnerDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCameraMode54Descriptor),
    RESOURCE_DESCRIPTOR_REF(gCameraMode55Descriptor),
    RESOURCE_DESCRIPTOR_REF(gCameraModeArwingDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCameraModeTitleDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDummy58Descriptor),
    RESOURCE_DESCRIPTOR_REF(gDll59ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gStaffCollisionResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll5BResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll5CResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll5DResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll5EResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll5FResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll60ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll61ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll62ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll63ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll64ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll65ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll66ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll67ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll68ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll69ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll6AResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll6BResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDummy6CDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll6DResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll6EResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll6FResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll70ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll71ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll72ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll73ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll74ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll75ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll76ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll77ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll78ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll79ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll7AResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll7BResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll7CResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll7DResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll7EResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll7FResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll80ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll81ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll82ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll83ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll84ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll85ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll86ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll87ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll88ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll89ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll8AResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll8BResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll8CResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll8DResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll8EResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll8FResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll90ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll91ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll92ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll93ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll94ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll95ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll96ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll97ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll98ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll99ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll9AResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll9BResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll9CResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll9DResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll9EResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll9FResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDllA0ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDllA1ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDllA2ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDllA3ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDummyA4ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDllA5ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDllA6ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDllA7ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDllA8ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDllA9ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDllAAResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gProjdummyResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gProjmagicstreamResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gProjmagicemmit1ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gProjroombeamResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gProjlightning1ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gProjlightning2ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gProjlightning3ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gProjrobotfireResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gProjlightning4ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gProjenergise1ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gProjenergise2ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gProjsquirt1ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gProjship1ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gProjlightning5ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gProjlightning7ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gProjlightning6ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gProjwallpowerResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gProjquakeshockResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gProjsunshockResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gProjteslaResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gProjcore1ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gProjcore2ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gProjcore3ResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gProjdfp1rResourceDescriptor),
    RESOURCE_DESCRIPTOR_REF(gPlayerObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gTrickyObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDllC5ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gAnimatedObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDIM2RoofRubObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDepthOfFieldPointObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gBaddieObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gIceBaddieObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDllCBObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gChukChukObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gIceBallObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDllCEObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCannonClawObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gGrimbleObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gTumbleWeedBushObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gTumbleweedObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDllD3ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSkeetlaWallObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gKaldachomObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gKaldachomMeObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gKaldachomSpObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gPinPonSpikeObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gPollenObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gPollenFragmentObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gMikaBombObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gMikaBombShadowObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gGCbaddieShieldObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gBaddieInterestPObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gHagabonObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSwarmBaddieObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWispBaddieObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gStaffObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gFireballObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gFlameThrowerspeObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gShieldObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gReStartMarkObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gFlammableVineObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCheckpoint4ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSetuppointObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSideloadObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSiderepelObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gInfoPointObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCollectibleObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gEffectBoxObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gPushableObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWarpPointObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gInvHitObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gIceblastObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gFlameblastObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDoorF4ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSidekickBallObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gAreaObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDllF7ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gLevelNameObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gProjectileSwitchObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gInvisibleHitSwitchObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gPressureSwitchFBObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDllFCObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDllFDObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gMagicPlantObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gMagicGemObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gTrickyWarpObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gTrickyGuardObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gStayPointObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCurveFishObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSmallBasketObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gLargeCrateObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gScarabObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWindLift107ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gEndObjectObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gBreakableCarryableObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gFall_LaddersObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gFireFlyLanternObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gLanternFireFlyObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gPortalSpellDoorObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDeathSeqObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gMMP_BridgeObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDoorObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDoorLockObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSeqObjectObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSeqObj2ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gIMMultiSeqObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll115ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWM_ColumnObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gAppleOnTreeObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDusterObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gColdWaterControlObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDecoration11AObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gLanded_ArwingObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gStaffActivatedObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gTreasureChestObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gMagicCaveBottomObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gMagicCaveTopObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gTrickyGuardSpotObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gInfoTextObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCCTestInfotObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gFuelCellObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDeathGasObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCurveObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gTriggerObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll127ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gKT_TorchObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCampFireObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCFCrateObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gFXEmitObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gTransporterObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gLFXEmitterObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCFLightWallObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gBarrelPadObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gAreaFXEmitObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCF_DoorLightObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWaterFallSprayObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSfxPlayerObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gTexscroll2ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gTexscrollObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWaveAnimatorObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gAlphaAnimatorObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gGroundAnimatorObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gHitAnimatorObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gVisAnimatorObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWallAnimatorObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gXYZAnimatorObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gExplodeAnimatorObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDIMBossIceSmashObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gTexFrameAnimatorObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gFogControlObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gLightningObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gFElevControlObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gFEseqobjectObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll144ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCloudPrisonControlObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll146ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll147ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCFGuardianObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWindLiftObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCFPowerBaseObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCFMainCrystalObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gBabyCloudRunnerObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll14DObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCFPrisonGuardObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCFPrisonUncleObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gGCRobotLightBeamObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll151ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll152ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCFPerchObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCFPrisonCageObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll155ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll156ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSpiritDoorSpiritObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gGunpowderBarrelObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gBlastedObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gExplodableObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCFForceFieldObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll15CObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSlidingDoorObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll15EObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gAttractorObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll160ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll161ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCFMagicWallObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll163ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCFLevelControlObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll165ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gExplodedObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSpiritDoorLockObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll168ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gIMIceMountainObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCRrockfallObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gMagicLightObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gIMSnowClawObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gIMIcePillarObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gIMAnimSpaceObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gIMSpaceThrusterObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gIMSpaceRingObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gIMSpaceRingGeneratorObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gLINKBLevelControlObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gLINKLevelControlObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCCRiverFlowObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDFropenodeObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll176ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll177ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDFSHShrineObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDFSHObjCreatorObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSpiritPrizeObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDFSHLaserBeamObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll17CObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gRollingBarrelObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gMMPLevelControlObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gMoonSeedBushObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gMMPAsteroidReObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gMMPTrenchFxObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gMMPMoonRockObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gMMPGeyserVentObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll184ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCCGasVentObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCCGasVentControlObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCCQueenObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCCLightfootObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCCSharpClawPadObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCCPedestalObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCCLevelControlObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gMMSHShrineObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gMMSHScalesObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gMMSHWaterSpikeObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gECSHShrineObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gECSHCupObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gECSHCreatorObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gGPSHShrineObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gGPSHObjCreatorObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gGPSHSceneObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDBSHShrineObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDBSHSymbolObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll197ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gNWSHLevelControlObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll199ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll19AObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll19BObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll19CObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll19DObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll19EObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gNWTreeBridgeObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gNWGeyserObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gNW_mammothObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gNWTrickyObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll1A3ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gNW_iceObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gNWLevelControlObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSHTrickyObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gEdibleMushroomObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gEnemyMushroomObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gBombPlantObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gBombPlantSporeObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gBombPlantingSpotObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSH_queenearthwalkerObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSH_thorntailObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSH_LevelControlObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWarpStoneLiftObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWarpStoneObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSH_staffObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSH_staffHazeObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSH_BeaconObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSH_EmptyTumbleWObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gLightfootObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSC_levelcontrolObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSC_MusicTreeObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSC_totempoleObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSC_CloudrunnerAObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSC_totempuzzleObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSC_totembondObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSC_totemstrengthObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gPaymentKioskObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gLavaBall1BEObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gLavaBall1BFObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDIMLogFireObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDIMSnowBallObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDIMSnowBall1C2ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDIMGateObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDIMIceWallObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDIMBarrierObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDIMCannonObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDIMLavaSmashObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDIMBridgeCogMaiObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDIMDismountPointObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gExplosionObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDIMWoodDoor2ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDIMMagicBridgeObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDIM_LevelControlObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll1CEObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll1CFObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDIM_trickyObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDIMTruthHornIceObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWorldPlanetObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWorldObjObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWorldAsteroidsObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDIM2ConveyorObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll1D6ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDIM2SnowBallObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDIM2PathGeneratorObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDIM2PrisonMammothObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll1DAObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll1DBObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDIM2IceFloeObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDIM2IcicleObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDIM2LavaControlObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll1DFObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDIM_BossObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDIM_BossGutObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDIM_BossTonsilObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDIM_BossGut2ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gMAGICMakerObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDIM_BossSpitObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDIMbosscrackparObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDIMbossfireObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSB_GalleonObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSB_PropellerObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSB_ShipHeadObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSB_ShipMastObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSB_ShipGunObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSB_FireBallObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSB_CannonBallObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSB_CloudBallObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSB_KyteCageObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSB_SeqDoorObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSB_CageKyteObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSB_MiniFireObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll1F4ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll1F5ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll1F6ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSB_ShipGunBrokeObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWM_GalleonObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWM_ObjCreatorObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWM_seqobjectObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll1FBObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gLaserBeamObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWM_LaserTargetObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gPressureSwitchObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll1FFObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll200ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWM_colriseObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll202ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll203ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWM_TorchObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll205ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gLightSourceObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWM_WormObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll208ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWM_LevelControlObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWM_GeneralScalesObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gFireFlyObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWM_spiritplaceObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWM_seqpointObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWM_sunObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWM_SpiritSetObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWM_PlanetsObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWM_WallCrawlerObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll212ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll213ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll214ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWM_newcrystalObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gVFP_LevelControlObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gVFP_ObjCreatorObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gVFP_MiniFireObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll219ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gVFP_statueballObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll21BObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gVFP_LaddersObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gVFPLiftObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gVFP_Block1ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gVFP_PlatformObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gVFP_DoorSwitchObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSeqPointObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gVFPDragHeadObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gVFP_coreplatObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSpellStoneUseObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gVFP_flamepointObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gVFP_lavapoolObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gVFP_lavastarObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gVfpSpellPlaceObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDFP_LevelControlObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDFP_ObjCreatorObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDFP_TorchObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll22CObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDFP_seqpointObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDoorswitchObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDfpfloorbarObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gChukaObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gTrickyCurveObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDFP_RotatePObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDfpstatue1ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDfperchwitchObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDfptargetblockObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gLaserUnsupportedObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDfpSpellPlaceObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gFireObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gTextBlockObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gPlatform1ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDfplightniObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDfppowerslObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll23DObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll23EObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDB_eggObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gGCRobotBlastObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDrakorEnergyObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDBstealerwormObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDBHoleControl1ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll244ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll245ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll246ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll247ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll248ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll249ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll24AObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll24BObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll24CObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gBossDrakorObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDrakorDThornBushObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gKtRexLevelObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gKtRexObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gKtRexFloorSwitchObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gKtLazerwallObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gKtLazerlightObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gKtFallingrocksObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSnowBikeObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDIMSnowHorn1ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDR_EarthWarriorObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDR_CloudRunnerObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSB_CloudRunnerObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gStaticCameraObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gMoonSeedPlantingSpotObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSnowClawObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCrCloudRaceObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSpellStoneObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCrFuelTankObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gProximityMineObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDrLaserCannonObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDrakorMissileObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gGmMazeWellObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll264ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDrCreatorObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gKytesMumObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll267ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDrCageControlObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gExplodePlanObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll26AObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDrChimmeyObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDrCageWithObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll26DObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDrShackleObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDrGeneratorObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll270ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDrakorHoverPadObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gHighTopObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gFirePipeObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll274ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll275ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll276ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll277ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll278ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDrEnergyDiscObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll27AObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll27BObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDrLightBeaObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll27DObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDrMusicContObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll27FObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDrCloudPerObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDrEarthCalObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gBarrelGenerObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDrBarrelGrObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gShopItemObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gShopObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gShopKeeperObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSPScarabObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSPDrapeObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSPitembeamObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gEarthWalkerObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll28BObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWCBouncyCraObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWCLevelContObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWCBeaconObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWCPressureSObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWCPushBlockObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWCTileObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWCTrexStatuObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSunTempleObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWCTempleObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWCApertureSObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWCTempleDiaObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWCTempleBriObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWCFloorTileObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll299ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gARWArwingObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gArwingAndrossStuffObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gARWArwingBoObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gARWArwingGuObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll29EObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gARWBombCollObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gRingObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gARWLevelConObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gARWSpeedStrObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll2A3ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDll2A4ObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gARWGeneratoObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gARWSquadronObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gARWProximitObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gARWBlockerObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gPointLightObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDirectionalLightObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gProjectedLightObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gControlLightObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gSoftBodyObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gWaterFlowWeObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gTreeObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gBrokenPipeObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCmbSrcObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gDustMoteSouObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gVortexObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCNTcounterObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gTimerObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gCNThitObjecObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gMCUpgradeObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gMCUpgradeMaObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gMCStaffEffeObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gMCLightningObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gGF_LevelConObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gAndrossObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gAndrossHandObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gAndrossBrainObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gAndrossLighObjDescriptor),
    RESOURCE_DESCRIPTOR_REF(gTitleScreenObjDescriptor),
    NULL,
};

s32 gModelEngineUiDllResourceIds[] = {
    -1, 16, 50, 51, 52, 53, 54, 55, 56, 57, -1, -1, 58, -1, 63, 64, 65, -1,
};
