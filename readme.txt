1. The program has hardcoded the MySQL login information as
   root, password as “12345” and the database as “SBA”. 
   If you will trial run the program, please change the 
   hostname to “localhost”
   username to “root”
   root password to “12345”
   create the database named “SBA”
   create the table named “record”

2. The ICT_SBA.exe should be placed in the cgi-bin folder.
   Then, start the web browser and browse "localhost/cgi-bin/ICT_SBA.exe"

3. The header of the program contains "mysql/mysql.h". Please modify
   the location of the header if a different location is used.

4. The libmysql.dll file should be placed in the same folder as the
   ICT_SBA.exe and the Database_set_up.exe.

5. When the program is run for the first time, please run the
   Database_set_up.exe first and select option 1 to set up the
   environment.