archive_name=docker-opencilk-v1.0.tar.gz
wget https://github.com/OpenCilk/opencilk-project/releases/download/opencilk%2Fv1.0/$archive_name
docker load -i $archive_name
rm $archive_name
