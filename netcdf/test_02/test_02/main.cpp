#include <stdlib.h>
#include <stdio.h>
#include <netcdf.h>
#include <fstream>

using namespace std;

#define FILE_NAME "E:/my_program/undergraduate/netcdf/cdf/oscar_vel2014.nc.gz.nc"

#define ERRCODE 2
#define ERR(e) {printf("Error: %s\n", nc_strerror(e)); system("pause");}
int main()
{
	/*int ncid, windDirID, windSpeedID, latID, lonID;
	float *windDirData = new float[NX*NY], *windSpeedData = new float[NX*NY];
	int *latData = new int[NX*NY], *lonData = new int[NX*NY];
	ofstream outfile("wind.txt");
	if (!outfile.is_open())
	{
		printf("file open error!\n");
		return 0;
	}*/

	int ncid;
	int uID, vID, timeID, depthID,latID,lonID;
	int x, y, retval;
	size_t time_len,depth_len,lat_len,lon_len;
	int time_offset = 0, lat_offset = 0, lon_offset = 0;

	if ((retval = nc_open(FILE_NAME, NC_NOWRITE, &ncid)))
		ERR(retval);

	if ((retval = nc_inq_varid(ncid, "u", &uID)))
		ERR(retval);
	if ((retval = nc_inq_varid(ncid, "v", &vID)))
		ERR(retval);

	if ((retval = nc_inq_dimid(ncid, "time", &timeID)))
		ERR(retval);
	if ((retval = nc_inq_dimlen(ncid, timeID, &time_len)))
		ERR(retval);
	if ((retval = nc_inq_dimid(ncid, "depth", &depthID)))
		ERR(retval);
	if ((retval = nc_inq_dimlen(ncid, depthID, &depth_len)))
		ERR(retval);
	if ((retval = nc_inq_dimid(ncid, "latitude", &latID)))
		ERR(retval);
	if ((retval = nc_inq_dimlen(ncid, latID, &lat_len)))
		ERR(retval);
	if ((retval = nc_inq_dimid(ncid, "longitude", &lonID)))
		ERR(retval);
	if ((retval = nc_inq_dimlen(ncid, lonID, &lon_len)))
		ERR(retval);


	printf("%d %d %d %d\n", depth_len, time_len, lat_len, lon_len);

	double *u_data = new double[time_len * lat_len * lon_len];
	double *v_data = new double[time_len * lat_len * lon_len];
	double *lat_data = new double[lat_len];
	double *lon_data = new double[lon_len];
	int *time_data = new int[time_len];


	if ((retval = nc_get_var_double(ncid, uID, &u_data[0])))
		ERR(retval);
	if ((retval = nc_get_var_double(ncid, vID, &v_data[0])))
		ERR(retval);
	if ((retval = nc_get_var_double(ncid, latID, &lat_data[0])))
		ERR(retval);
	if ((retval = nc_get_var_double(ncid, lonID, &lon_data[0])))
		ERR(retval);
	if ((retval = nc_get_var_int(ncid, timeID, &time_data[0])))
		ERR(retval);

	ofstream outfile("current_20140717.txt");
	if (!outfile.is_open())
	{
		printf("file open error!\n");
		return 0;
	}

	for (time_offset = 39; time_offset < 44; time_offset++){
		outfile << "time_offset:" << time_offset << "\n";
		for (lat_offset = 0; lat_offset < lat_len; lat_offset++){
			for (lon_offset = 0; lon_offset < lon_len; lon_offset++){
				double latCur = *(lat_data + lat_offset);
				double lonCur = *(lon_data + lon_offset);
				if (lonCur < 110 && lonCur >80 && latCur < 0 && latCur > -35){
					outfile << *(time_data + time_offset) << " "\
						<<latCur<< " " << lonCur <<" "\
						<< *(u_data + time_offset * lat_len * lon_len + lat_offset * lon_len + lon_offset) << " "\
						<< *(v_data + time_offset * lat_len * lon_len + lat_offset * lon_len + lon_offset)<< "\n";
				}
			}
		}
	}

	outfile.close();
	if ((retval = nc_close(ncid)))
		ERR(retval);
	printf("*** SUCCESS reading example file %s!\n", FILE_NAME);
	system("pause");
	return 0;
}