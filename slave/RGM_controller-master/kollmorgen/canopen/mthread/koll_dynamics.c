# include "koll_dynamics.h"



 /*******************************************
* gravity_compensation                 
* para@(p[],gra_compen[])
*******************************************/
void koll_gravity_compensation(double p[7]) 
{
  double q1,q2,q3,q4,q5,q6;
  q1=p[1]*pi/180;
  q2=p[2]*pi/180;
  q3=p[3]*pi/180;
  q4=p[4]*pi/180;
  q5=p[5]*pi/180;
  q6=p[6]*pi/180;
  double t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,t19,t20,t21;
  double t22,t23,t24,t25,t26,t27,t28,t29,t30,t31,t32,t33,t34,t35,t36,t37,t38,t39,t40,t41;
  double t42,t43,t44,t45,t46,t47,t48,t49,t50,t51,t52,t53,t54,t55,t56,t57,t58,t59,t60,t61;
  double t62,t63,t64,t65,t66,t67,t68,t69,t70,t71,t72,t73,t74,t75,t76,t77,t78,t79,t80,t81;
  double t82,t83,t84,t85,t86,t87,t88,t89,t90,t91,t92,t93,t94,t95,t96,t97,t98,t99,t100;
  double t101,t102,t103,t104,t105,t106,t107,t108,t109,t110,t111,t112,t113,t114,t115,t116,t117,t118,t119,t120;
  double t121,t122,t123,t124,t125,t126,t127,t128,t129,t130,t131,t132,t133,t134,t135,t136,t137,t138,t139,t140;
  double t141,t142,t143,t144,t145,t146,t147,t148,t149,t150,t151,t152,t153,t154,t155,t156,t157,t158,t159,t160,t161;
  t2 = cos(q5);
  t3 = cos(q4);
  t4 = cos(q2);
  t5 = sin(q3);
  t6 = cos(q3);
  t7 = sin(q2);
  t8 = cos(q6);
  t9 = t4*t6*9.8015;
  t15 = t5*t7*9.8015;
  t10 = t9-t15;
  t11 = sin(q4);
  t12 = t4*t5*9.8015;
  t13 = t6*t7*9.8015;
  t14 = t12+t13;
  t16 = t3*t10;
  t17 = t11*t14;
  t18 = sin(q5);
  t19 = t3*t14;
  t20 = t10*t11;
  t21 = t19+t20;
  t22 = t16-t17;
  t23 = sin(q6);
  t24 = t18*6.001687800921391E-16;
  t25 = t2*t21;
  t26 = t18*t22*6.123233995736766E-17;
  t27 = t24+t25+t26;
  t28 = t2*6.001687800921391E-16;
  t29 = t2*t22*6.123233995736766E-17;
  t33 = t18*t21;
  t30 = t28+t29-t33;
  t31 = -t16+t17+3.674973877440049E-32;
  t32 = t8*t27*(1.19E2/5.0E2);
  t34 = t23*t30*1.45732969098535E-17;
  t39 = t23*t31*(1.19E2/5.0E2);
  t35 = t32+t34-t39;
  t36 = t23*t27*(1.19E2/5.0E2);
  t37 = t8*t31*(1.19E2/5.0E2);
  t40 = t8*t30*1.45732969098535E-17;
  t38 = t36+t37-t40;
  t41 = t2*1.043093339800138E-15;
  t42 = t2*t22*1.06421806845905E-16;
  t43 = t23*t35*6.123233995736766E-17;
  t44 = t11*t14*1.45732969098535E-17;
  t56 = t8*t38*6.123233995736766E-17;
  t57 = t18*t21*(8.69E2/5.0E2);
  t58 = t3*t10*1.45732969098535E-17;
  t45 = t41+t42+t43+t44-t56-t57-t58+5.355648545188942E-49;
  t46 = t2*8.746437828307318E-33;
  t47 = t2*t22*8.923570706838053E-34;
  t48 = t8*t38;
  t49 = t18*9.002531701382087E-16;
  t50 = t2*t21*(3.0/2.0);
  t51 = t18*t22*9.184850993605149E-17;
  t52 = t8*t35;
  t53 = t23*t38;
  t54 = t49+t50+t51+t52+t53;
  t55 = t11*t14*3.0;
  t90 = t18*t21*1.45732969098535E-17;
  t91 = t23*t35;
  t92 = t18*t54*6.123233995736766E-17;
  t93 = t3*t10*3.0;
  t94 = t2*t45*6.123233995736766E-17;
  t59 = t46+t47+t48+t55-t90-t91-t92-t93-t94+5.512460816160074E-32;
  t60 = t2*1.085585289430661E-20;
  t61 = t2*t22*1.107570565148866E-21;
  t62 = t8*t27*5.253374E-3;
  t63 = t23*t30*3.216763826911964E-19;
  t64 = t11*t14*1.107570565148866E-21;
  t80 = t18*t21*1.8088E-5;
  t81 = t23*t31*5.253374E-3;
  t82 = t3*t10*1.107570565148866E-21;
  t65 = t60+t61+t62+t63+t64-t80-t81-t82+4.070292894343596E-53;
  t66 = t2*3.271039885258177E-20;
  t67 = t2*t22*3.337284992356452E-21;
  t68 = t8*t31*5.253374E-3;
  t69 = t11*t14*3.337284992356452E-21;
  t70 = t23*t27*5.253374E-3;
  t72 = t18*t21*5.4502E-5;
  t73 = t8*t30*3.216763826911964E-19;
  t74 = t3*t10*3.337284992356452E-21;
  t71 = t66+t67+t68+t69+t70-t72-t73-t74+1.226443516848268E-52;
  t75 = t8*t35*5.725223786013876E-18;
  t76 = t23*t38*5.725223786013876E-18;
  t77 = t8*t30*1.107570565148866E-21;
  t78 = t8*t27*5.4502E-5;
  t79 = t23*t30*3.337284992356452E-21;
  t100 = t8*t31*1.8088E-5;
  t101 = t23*t31*5.4502E-5;
  t102 = t23*t71*6.123233995736766E-17;
  t103 = t23*t27*1.8088E-5;
  t104 = t8*t65*6.123233995736766E-17;
  t83 = t75+t76+t77+t78+t79-t100-t101-t102-t103-t104;
  t84 = t2*5.017826918641907E-50;
  t85 = t23*t65;
  t86 = t2*t22*5.119447960661029E-51;
  t87 = t8*t38*9.35E-2;
  t88 = t11*t14*5.119447960661029E-51;
  t105 = t18*t21*8.360693000178317E-35;
  t106 = t23*t35*9.35E-2;
  t107 = t8*t71;
  t108 = t3*t10*5.119447960661029E-51;
  t89 = t84+t85+t86+t87+t88-t105-t106-t107-t108+1.881383752234302E-82;
  t95 = t2*1.379944572539562E-66;
  t96 = t2*t22*1.407891213119994E-67;
  t97 = t8*t38*1.577721810442024E-34;
  t98 = t18*t54*1.023E-1;
  t99 = t11*t14*2.366582715663035E-34;
  t109 = t2*t45*1.023E-1;
  t110 = t18*t83;
  t129 = t18*t21*2.299260838472322E-51;
  t130 = t23*t35*1.577721810442024E-34;
  t131 = t3*t10*2.366582715663035E-34;
  t132 = t2*t89;
  t111 = t95+t96+t97+t98+t99+t109+t110-t129-t130-t131-t132+8.697129658862787E-66;
  t112 = t8*t31*1.107570565148866E-21;
  t113 = t8*t35*9.35E-2;
  t114 = t23*t38*9.35E-2;
  t115 = t23*t31*3.337284992356452E-21;
  t116 = t2*t54*6.264068377638711E-18;
  t117 = t2*t83*6.123233995736766E-17;
  t118 = t23*t27*1.107570565148866E-21;
  t119 = t18*t89*6.123233995736766E-17;
  t134 = t18*t45*6.264068377638711E-18;
  t135 = t23*t71;
  t136 = t8*t30*6.781913737196921E-38;
  t137 = t8*t27*3.337284992356452E-21;
  t138 = t23*t30*2.043497691865914E-37;
  t139 = t8*t65;
  t120 = t112+t113+t114+t115+t116+t117+t118+t119-t134-t135-t136-t137-t138-t139;
  t121 = t2*t54;
  t122 = t3*t14*(3.0/2.0);
  t123 = t10*t11*(3.0/2.0);
  t125 = t18*t45;
  t124 = t121+t122+t123-t125;
  t126 = t2*1.972485359193087E-49;
  t127 = t2*t22*2.012432137114816E-50;
  t128 = t8*t38*2.255187080629851E-17;
  t133 = t18*t54*3.683E-1;
  t140 = t11*t14*3.382780620944776E-17;
  t141 = t2*t45*3.683E-1;
  t142 = t4*t6*3.4980534144E-1;
  t143 = t18*(t75+t76+t77+t78+t79-t100-t101-t102-t103-t104);
  t144 = t95+t96+t97+t98+t99+t109-t129-t130-t131-t132+t143+8.697129658862787E-66;
  t145 = t11*t120;
  t146 = t11*t124*1.173E-1;
  t147 = t4*t6*2.509184E1;
  t148 = t11*t124;
  t149 = t11*t59;
  t150 = t4*t5*2.509184E1;
  t151 = t6*t7*2.509184E1;
  t152 = t3*t124;
  t153 = t149+t150+t151+t152;
  t154 = t4*t6*6.19394579584;
  t155 = t2*t54*1.023E-1;
  t156 = t8*t30*4.152724475175836E-54;
  t157 = t11*t124*3.683E-1;
  t158 = t8*t27*2.043497691865914E-37;
  t159 = t23*t30*1.251281453704298E-53;
  t160 = t2*(t75+t76+t77+t78+t79-t100-t101-t102-t103-t104);
  t161 = t18*t89;
  model_compen[1] = t4*7.471861244635095E-16+t119+t4*t6*3.79269794648383E-16-t5*t7*3.79269794648383E-16+t8*t27*1.251281453704298E-53+t8*t30*2.54281036813248E-70-t8*t31*4.152724475175836E-54-t8*t35*3.505688491972092E-34-t23*t27*4.152724475175836E-54+t23*t30*7.661889135557078E-70-t23*t31*1.251281453704298E-53+t2*t54*6.264068377638712E-18-t23*t38*3.505688491972092E-34-t3*t59*2.255187080629851E-17-t18*t45*6.264068377638712E-18+t8*t65*3.749399456654644E-33+t23*t71*3.749399456654644E-33+t11*t124*2.255187080629851E-17+t5*t153*2.459090772687885E-17+t2*(t75+t76+t77+t78+t79-t100-t101-t102-t103-t104)*6.123233995736766E-17-t7*(t4*7.28016214-t5*(t126+t127+t128+t133+t140+t141-t4*t5*3.4980534144E-1-t3*t10*3.382780620944776E-17-t6*t7*3.4980534144E-1-t18*t21*3.286551091328455E-34-t23*t35*2.255187080629851E-17-t11*t59*1.173E-1-t3*t120-t3*t124*1.173E-1+t11*t144+7.108330372102853E-16)+t6*(t142+t145+t146-t5*t7*3.4980534144E-1-t3*t59*1.173E-1+t3*t144))+t6*(t147+t148-t5*t7*2.509184E1-t3*t59)*2.459090772687885E-17-t4*(t2*2.150828455747879E-49-t7*7.28016214-t3*t10*3.688636159031828E-17+t2*t22*2.194387038461337E-50+t11*t14*3.688636159031828E-17-t18*t21*3.583705995866162E-34+t8*t38*2.459090772687885E-17+t2*t45*(2.51E2/6.25E2)-t23*t35*2.459090772687885E-17+t18*t54*(2.51E2/6.25E2)+t6*(t126+t127+t128+t133+t140+t141-t4*t5*3.4980534144E-1-t3*t10*3.382780620944776E-17-t6*t7*3.4980534144E-1-t18*t21*3.286551091328455E-34-t23*t35*2.255187080629851E-17-t11*t59*1.173E-1+t11*t111-t3*t120-t3*t124*1.173E-1+7.108330372102853E-16)+t5*(t142+t145+t146-t5*t7*3.4980534144E-1-t3*t59*1.173E-1+t3*t111)+1.725758919728622E-15);
  model_compen[2] = t4*1.220247544E1+t102+t104+t154+t155+t156+t157+t158+t159+t160+t161-t5*t7*6.19394579584-t8*t31*6.781913737196921E-38-t8*t35*5.725223786013876E-18-t23*t27*6.781913737196921E-38-t23*t31*2.043497691865914E-37-t23*t38*5.725223786013876E-18-t3*t59*3.683E-1-t18*t45*1.023E-1+t5*t153*(2.51E2/6.25E2)+t6*(t147+t148-t5*t7*2.509184E1-t3*t59)*(2.51E2/6.25E2);
  model_compen[3] = t102+t104+t154+t155+t156+t157+t158+t159+t160+t161-t5*t7*6.19394579584-t8*t31*6.781913737196921E-38-t8*t35*5.725223786013876E-18-t23*t27*6.781913737196921E-38-t23*t31*2.043497691865914E-37-t23*t38*5.725223786013876E-18-t3*t59*3.683E-1-t18*t45*1.023E-1;
  model_compen[4] = t102+t104+t155+t156+t158+t159+t160+t161-t8*t31*6.781913737196921E-38-t8*t35*5.725223786013876E-18-t23*t27*6.781913737196921E-38-t23*t31*2.043497691865914E-37-t23*t38*5.725223786013876E-18-t18*t45*1.023E-1;
  model_compen[5] = -t112-t113-t114-t115-t118+t135+t136+t137+t138+t139;
  model_compen[6] = t77+t78+t79-t100-t101-t103;
}


