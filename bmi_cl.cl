__kernel void bmi_cl(__global float *average,float tall,float weight)
{
  tall/=100;
  *average =weight/(tall*tall);
}
