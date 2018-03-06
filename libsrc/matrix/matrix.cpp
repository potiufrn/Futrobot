#define TIPODADO double
#define TIPOVETOR vvector
#define TIPOFVETOR fvector
#define TIPOMATRIZ vmatrix
#define TIPOFMATRIZ fmatrix

#include "tmatrix.cpp"

const TIPOMATRIZ nullmatrix(unsigned lin, unsigned col)
{
  TIPOMATRIZ prov(lin,col);
  for (unsigned i=0; i<prov.numlines(); i++) {
    prov.x[i] = nullvector(col);
  }
  prov.maketemp();
  return(prov);
}

const TIPOMATRIZ identity(unsigned dim)
{
  TIPOMATRIZ prov(dim,dim);
  for (unsigned i=0; i<prov.numlines(); i++)
    for (unsigned j=0; j<prov.numcolumns(); j++)
      prov.x[i][j] = (i==j ? (TIPODADO)1.0 : (TIPODADO)0.0);
  prov.maketemp();
  return(prov);
}

// Decomposição em valores singulares

inline double fmax(double x, double y){return(x>y ? x : y);}
inline unsigned imin(unsigned i, unsigned j){return(i<j ? i : j);}

void vmatrix::singularvalues(vmatrix &U, vvector &W, vmatrix &V) const
{
  const unsigned m = numlines();
  const unsigned n = numcolumns();
  U = *this;
  W = vvector(n);
  V = vmatrix(n,n);
  unsigned i,j,jj,k,l,nm;
  int flag,its;
  double c,g,h,f,s,x,y,z;
  vvector rv1(n);
  double anorm,scale;

  g=0.0;
  scale=anorm=0.0;
  for (i=0; i<n; i++) {
    l=i+1;
    rv1[i]=scale*g;
    g=s=0.0;
    scale=0.0;
    if (i < m) {
      //Householder reduction to bidiagonal form.
      for (k=i;k<m;k++) scale += modulo(U.x[k][i]);
      if (scale) {
	for (k=i;k<m;k++) {
	  U.x[k][i] /= scale;
	  s += U.x[k][i]*U.x[k][i];
	}
	f=U.x[i][i];
	g = -sign(sqrt(s),f);
	h=f*g-s;
	U.x[i][i]=f-g;
	for (j=l;j<n;j++) {
	  for (s=0.0,k=i;k<m;k++) s += U.x[k][i]*U.x[k][j];
	  f=s/h;
	  for (k=i;k<m;k++) U.x[k][j] += f*U.x[k][i];
	}
	for (k=i;k<m;k++) U.x[k][i] *= scale;
      }
    }
    W[i]=scale*g;
    g=s=0.0;
    scale=0.0;
    if (i < m && i != (n-1)) {
      for (k=l; k<n; k++) scale += modulo(U.x[i][k]);
      if (scale) {
	for (k=l; k<n; k++) {
	  U.x[i][k] /= scale;
	  s += U.x[i][k]*U.x[i][k];
	}
	f=U.x[i][l];
	g = -sign(sqrt(s),f);
	h=f*g-s;
	U.x[i][l]=f-g;
	for (k=l; k<n; k++) rv1[k]=U.x[i][k]/h;
	for (j=l; j<m; j++) {
	  for (s=0.0,k=l; k<n; k++) s += U.x[j][k]*U.x[i][k];
	  for (k=l; k<n; k++) U.x[j][k] += s*rv1[k];
	}
	for (k=l; k<n; k++) U.x[i][k] *= scale;
      }
    }
    anorm = fmax(anorm, (modulo(W[i])+modulo(rv1[i])));
  }

  for (i=n-1; i<n; i--) {
    if (i < n-1) {
      if (g != 0.0) {
	for (j=l; j<n; j++)
	  V.x[j][i]=(U.x[i][j]/U.x[i][l])/g;
	for (j=l; j<n; j++) {
	  for (s=0.0,k=l; k<n; k++) s += U.x[i][k]*V.x[k][j];
	  for (k=l; k<n; k++) V.x[k][j] += s*V.x[k][i];
	}
      }
      for (j=l; j<n; j++) V.x[i][j]=V.x[j][i]=0.0;
    }
    V.x[i][i]=1.0;
    g=rv1[i];
    l=i;
  }
  for (i=imin(m,n)-1; i<imin(m,n); i--) {
    l=i+1;
    g=W[i];
    for (j=l; j<n; j++) U.x[i][j]=0.0;
    if (g != 0.0) {
      g=1.0/g;
      for (j=l; j<n; j++) {
	for (s=0.0,k=l; k<m; k++) s += U.x[k][i]*U.x[k][j];
	f=(s/U.x[i][i])*g;
	for (k=i; k<m; k++) U.x[k][j] += f*U.x[k][i];
      }
      for (j=i; j<m; j++) U.x[j][i] *= g;
    }
    else for (j=i; j<m; j++) U.x[j][i]=0.0;
    //++U.x[i][i];
    U.x[i][i] += 1.0;;
  }
  for (k=n-1; k<n; k--) {
    for (its=1; its<=30; its++) {
      flag=1;
      for (l=k; l<=k; l--) {
	nm=l-1;
	if ((modulo(rv1[l])+anorm) == anorm) {
	  flag=0;
	  break;
	}
	if ((modulo(W[nm])+anorm) == anorm) break;
      }
      if (flag) {
	c=0.0;
	s=1.0;
	for (i=l; i<=k; i++) {
	  f=s*rv1[i];
	  rv1[i]=c*rv1[i];
	  if ((modulo(f)+anorm) == anorm) break;
	  g=W[i];
	  h=hypot(f,g);
	  W[i]=h;
	  h=1.0/h;
	  c=g*h;
	  s = -f*h;
	  for (j=0; j<m; j++) {
	    y=U.x[j][nm];
	    z=U.x[j][i];
	    U.x[j][nm]=y*c+z*s;
	    U.x[j][i]=z*c-y*s;
	  }
	}
      }
      z=W[k];
      if (l == k) {
	if (::real(z) < 0.0) {
	  W[k] = -z;
	  for (j=0;j<n;j++) V.x[j][k] = -V.x[j][k];
	}
	break;
      }
      if (its == 30) {
	matrix_error_int(MATRIX_NO_CONVERGENCE);
	return;
      }
      x=W[l];
      nm=k-1;
      y=W[nm];
      g=rv1[nm];
      h=rv1[k];
      f=((y-z)*(y+z)+(g-h)*(g+h))/(2.0*h*y);
      g=hypot(f,1.0);
      f=((x-z)*(x+z)+h*((y/(f+sign(g,f)))-h))/x;
      c=s=1.0;
      for (j=l; j<=nm; j++) {
	i=j+1;
	g=rv1[i];
	y=W[i];
	h=s*g;
	g=c*g;
	z=hypot(f,h);
	rv1[j]=z;
	c=f/z;
	s=h/z;
	f=x*c+g*s;
	g = g*c-x*s;
	h=y*s;
	y *= c;
	for (jj=0; jj<n; jj++) {
	  x=V.x[jj][j];
	  z=V.x[jj][i];
	  V.x[jj][j]=x*c+z*s;
	  V.x[jj][i]=z*c-x*s;
	}
	z=hypot(f,h);
	W[j]=z;
	if (z != 0.0) {
	  z=1.0/z;
	  c=f*z;
	  s=h*z;
	}
	f=c*g+s*y;
	x=c*y-s*g;
	for (jj=0; jj<m; jj++) {
	  y=U.x[jj][j];
	  z=U.x[jj][i];
	  U.x[jj][j]=y*c+z*s;
	  U.x[jj][i]=z*c-y*s;
	}
      }
      rv1[l]=0.0;
      rv1[k]=f;
      W[k]=x;
    }
  }
}
