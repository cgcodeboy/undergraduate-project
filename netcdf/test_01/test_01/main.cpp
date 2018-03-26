#include <stdlib.h>
#include <stdio.h>
#include <netcdf.h>
#include <fstream>

using namespace std;

#define FILE_NAME "E:/my_program/undergraduate/netcdf/cdf/ascat_20140717_005700_metopa_40168_eps_o_250_2300_ovw.l2.nc.gz.nc"

/* We are reading 2D data, a 42*1632 grid. */
#define NX 1632
#define NY 42
/* Handle errors by printing an error message and exiting with a
* non-zero status. */
#define ERRCODE 2
#define ERR(e) {printf("Error: %s\n", nc_strerror(e)); system("pause");}
int main()
{
	int ncid, windDirID, windSpeedID, latID, lonID;
	float *windDirData = new float[NX*NY], *windSpeedData = new float[NX*NY];
	int *latData = new int[NX*NY], *lonData = new int[NX*NY];
	ofstream outfile("wind_20140717.txt");
	if (!outfile.is_open())
	{
		printf("file open error!\n");
		return 0;
	}

	int x, y, retval;

	//open file
	if ((retval = nc_open(FILE_NAME, NC_NOWRITE, &ncid)))
		ERR(retval);

	//get wind direction data
	if ((retval = nc_inq_varid(ncid, "wind_dir", &windDirID)))
		ERR(retval);
	if ((retval = nc_get_var_float(ncid, windDirID, &windDirData[0])))
		ERR(retval);

	//get wind speed data
	if ((retval = nc_inq_varid(ncid, "wind_speed", &windSpeedID)))
		ERR(retval);
	if ((retval = nc_get_var_float(ncid, windSpeedID, &windSpeedData[0])))
		ERR(retval);

	//get lon data
	int lon_len, lat_len;
	if ((retval = nc_inq_dimid(ncid, "longitude", &lon_len)))
		ERR(retval);
	
	/*if ((retval = nc_inq_varid(ncid, "lon", &lonID)))
		ERR(retval);
	if ((retval = nc_get_var_int(ncid, lonID, &lonData[0])))
		ERR(retval);*/

	//get lon data
	if ((retval = nc_inq_dimid(ncid, "latitude", &lat_len)))
		ERR(retval);
	printf("%d %d", lat_len, lon_len);
	/*if ((retval = nc_inq_varid(ncid, "lat", &latID)))
		ERR(retval);
	if ((retval = nc_get_var_int(ncid, latID, &latData[0])))
		ERR(retval);*/


	/* write the data. */
	/*outfile << "longitude  latitude  wind_dir  wind_speed\n";
	for (x = 0; x < NX; x++){
		for (y = 0; y < NY; y++){
			outfile << *(latData + x* NY + y) * pow(10, -5) << " " << *(lonData + x* NY + y) * pow(10, -5) << " ";
			if (*(windDirData + x* NY + y) >0 && *(windDirData + x* NY + y) < 3600)
				outfile << *(windDirData + x* NY + y) * 0.1 << " ";
			else{
				outfile << "nan ";
			}

			if (*(windSpeedData + x* NY + y) > 0 && *(windSpeedData + x* NY + y) < 5000)
				outfile << *(windSpeedData + x* NY + y)* 0.01 << " ";
			else{
				outfile << "nan ";
			}
			outfile << "\n";
		}
		outfile << "\n\n";
	}*/
	/* Close the file, freeing all resources. */
	if ((retval = nc_close(ncid)))
		ERR(retval);
	printf("*** SUCCESS reading example file %s!\n", FILE_NAME);

	system("pause");
	return 0;
}