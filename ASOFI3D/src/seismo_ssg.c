/*------------------------------------------------------------------------
 *   store amplitudes (particle velocities or pressure) at receiver positions
     in arrays
 *  ----------------------------------------------------------------------*/

#include "fd.h"
#include "data_structures.h"

void seismo(int lsamp, int ntr, int **recpos, float **sectionvx, float **sectionvy, 
        float **sectionvz, float **sectiondiv, float **sectioncurl, float **sectionp,
        Velocity *v,
        Tensor3d *s, float ***pi, float ***u) {

	extern int SEISMO;
	int i, j, k, itr, ins, nxrec, nyrec, nzrec;
	float amp, dh24x, dh24y, dh24z, vyx, vxy, vxx, vyy, vzx, vyz, vxz, vzy, vzz;
	extern float DX, DY, DZ;
        float ***vx = v->x;
        float ***vy = v->y;
        float ***vz = v->z;

        float ***sxx = s->xx;
        float ***syy = s->yy;
        float ***szz = s->zz;

	ins=lsamp; /* changed from "ins=lsamp/NDT;" (neccessary after correction of the buggy ns in sofi3D.c) */
	dh24x=1.0/DX;
	dh24y=1.0/DY;
	dh24z=1.0/DZ;

	for (itr=1;itr<=ntr;itr++){
		nxrec=recpos[1][itr];
		nyrec=recpos[2][itr];
		nzrec=recpos[3][itr];
		switch (SEISMO){                          
		case 1 : /* particle velocities */
			sectionvx[itr][ins]=vx[nyrec][nxrec][nzrec];
			sectionvy[itr][ins]=vy[nyrec][nxrec][nzrec];
			sectionvz[itr][ins]=vz[nyrec][nxrec][nzrec];
			break;
		case 2 : /* pressure*/
			sectionp[itr][ins]=(-sxx[nyrec][nxrec][nzrec]-syy[nyrec][nxrec][nzrec]-szz[nyrec][nxrec][nzrec])/3;
			break;
		case 3 : /* curl +div */
			i=nxrec; j=nyrec; k=nzrec;
			/*vxy=(-vx[j+2][i][k]+27.0*(vx[j+1][i][k]-vx[j][i][k])+vx[j-1][i][k])*(24.0*DY);
			vxz=(-vx[j][i][k+2]+27.0*(vx[j][i][k+1]-vx[j][i][k])+vx[j][i][k-1])*(24.0*DZ);
			vyx=(-vy[j][i+2][k]+27.0*(vy[j][i+1][k]-vy[j][i][k])+vy[j][i-1][k])*(24.0*DX);
			vyz=(-vy[j][i][k+2]+27.0*(vy[j][i][k+1]-vy[j][i][k])+vy[j][i][k-1])*(24.0*DZ);
			vzx=(-vz[j][i+2][k]+27.0*(vz[j][i+1][k]-vz[j][i][k])+vz[j][i-1][k])*(24.0*DX);
			vzy=(-vz[j+2][i][k]+27.0*(vz[j+1][i][k]-vz[j][i][k])+vz[j-1][i][k])*(24.0*DY);*/

			vxy=(vx[j+1][i][k]-vx[j][i][k])*(dh24y);
			vxz=(vx[j][i][k+1]-vx[j][i][k])*(dh24z);
			vyx=(vy[j][i+1][k]-vy[j][i][k])*(dh24x);
			vyz=(vy[j][i][k+1]-vy[j][i][k])*(dh24z);
			vzx=(vz[j][i+1][k]-vz[j][i][k])*(dh24x);
			vzy=(vz[j+1][i][k]-vz[j][i][k])*(dh24y);

			amp=u[j][i][k]*((vyz-vzy)*fabs(vyz-vzy)+
					(vzx-vxz)*fabs(vzx-vxz)+(vxy-vyx)*fabs(vxy-vyx));
			sectioncurl[itr][ins]=fsign(amp)*sqrt(fabs(amp));

			/*vxx=(-vx[j][i+1][k]+27.0*(vx[j][i][k]-vx[j][i-1][k])+vx[j][i-2][k])*(24.0*DX);
			vyy=(-vy[j+1][i][k]+27.0*(vy[j][i][k]-vy[j-1][i][k])+vy[j-2][i][k])*(24.0*DY);
			vzz=(-vz[j][i][k+1]+27.0*(vz[j][i][k]-vz[j][i][k-1])+vz[j][i][k-2])*(24.0*DZ);*/

			vxx=(vx[j][i][k]-vx[j][i-1][k])*(dh24x);
			vyy=(vy[j][i][k]-vy[j-1][i][k])*(dh24y);
			vzz=(vz[j][i][k]-vz[j][i][k-1])*(dh24z);

			sectiondiv[itr][ins]=(vxx+vyy+vzz)*sqrt(pi[j][i][k]);

			break;
		case 4 : /* all */

			sectionvx[itr][ins]=vx[nyrec][nxrec][nzrec];
			sectionvy[itr][ins]=vy[nyrec][nxrec][nzrec];
			sectionvz[itr][ins]=vz[nyrec][nxrec][nzrec]; 
			sectionp[itr][ins]=(-sxx[nyrec][nxrec][nzrec]-syy[nyrec][nxrec][nzrec]-szz[nyrec][nxrec][nzrec])/3;
			i=nxrec; j=nyrec; k=nzrec;

			/*vxy=(-vx[j+2][i][k]+27.0*(vx[j+1][i][k]-vx[j][i][k])+vx[j-1][i][k])*(dh24y);
			vxz=(-vx[j][i][k+2]+27.0*(vx[j][i][k+1]-vx[j][i][k])+vx[j][i][k-1])*(dh24z);
			vyx=(-vy[j][i+2][k]+27.0*(vy[j][i+1][k]-vy[j][i][k])+vy[j][i-1][k])*(dh24x);
			vyz=(-vy[j][i][k+2]+27.0*(vy[j][i][k+1]-vy[j][i][k])+vy[j][i][k-1])*(dh24z);
			vzx=(-vz[j][i+2][k]+27.0*(vz[j][i+1][k]-vz[j][i][k])+vz[j][i-1][k])*(dh24x);
			vzy=(-vz[j+2][i][k]+27.0*(vz[j+1][i][k]-vz[j][i][k])+vz[j-1][i][k])*(dh24y);*/

			vxy=(vx[j+1][i][k]-vx[j][i][k])*(dh24y);
			vxz=(vx[j][i][k+1]-vx[j][i][k])*(dh24z);
			vyx=(vy[j][i+1][k]-vy[j][i][k])*(dh24x);
			vyz=(vy[j][i][k+1]-vy[j][i][k])*(dh24z);
			vzx=(vz[j][i+1][k]-vz[j][i][k])*(dh24x);
			vzy=(vz[j+1][i][k]-vz[j][i][k])*(dh24y);

			amp=u[j][i][k]*((vyz-vzy)*fabs(vyz-vzy)+
					(vzx-vxz)*fabs(vzx-vxz)+(vxy-vyx)*fabs(vxy-vyx));
			sectioncurl[itr][ins]=fsign(amp)*sqrt(fabs(amp));

			/*vxx=(-vx[j][i+1][k]+27.0*(vx[j][i][k]-vx[j][i-1][k])+vx[j][i-2][k])*(dh24x);
			vyy=(-vy[j+1][i][k]+27.0*(vy[j][i][k]-vy[j-1][i][k])+vy[j-2][i][k])*(dh24y);
			vzz=(-vz[j][i][k+1]+27.0*(vz[j][i][k]-vz[j][i][k-1])+vz[j][i][k-2])*(dh24z);*/

			vxx=(vx[j][i][k]-vx[j][i-1][k])*(dh24x);
			vyy=(vy[j][i][k]-vy[j-1][i][k])*(dh24y);
			vzz=(vz[j][i][k]-vz[j][i][k-1])*(dh24z);

			sectiondiv[itr][ins]=(vxx+vyy+vzz)*sqrt(pi[j][i][k]);
			break;

		}
	}
}
