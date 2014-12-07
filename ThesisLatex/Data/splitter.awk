#! /bin/awk

{
for (i=1;i<9;++i)
{
	archivo=FILENAME"_"i;
	print NR" "$i>archivo;
};

}
