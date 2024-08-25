# Use an official R base image
FROM r-base:latest

# Install system dependencies
RUN apt-get update && apt-get install -y \
    libcurl4-openssl-dev \
    libssl-dev \
    libxml2-dev \
    libfontconfig1-dev \
    libharfbuzz-dev \
    libfribidi-dev \
    libfreetype6-dev \
    libpng-dev \
    libtiff5-dev \
    libjpeg-dev \
    libnetcdf-dev \
    default-jdk \
    libmagick++-dev \
    libglu1-mesa-dev \
    libgl1-mesa-dev \
    libudunits2-dev \
    libgdal-dev

# Install Shiny
RUN R -e "install.packages('shiny', repos='http://cran.rstudio.com/')"

# Install BiocManager and remotes
RUN R -e "install.packages(c('BiocManager', 'remotes'), repos='http://cran.rstudio.com/')"

# Install StreamFind
RUN R -e "BiocManager::install('odea-project/StreamFind', dependencies = TRUE)"

# Install StreamFindData
RUN R -e "BiocManager::install('odea-project/StreamFindData')"

# Install patRoon dependencies
RUN R -e "BiocManager::install(c('ncdf4', 'rJava', 'magick', 'mzR', 'rcdklibs', 'MSnbase', 'rcdk', 'xcms', 'CAMERA'), dependencies = TRUE)"

# Install patRoon
RUN R -e "BiocManager::install('rickhelmus/patRoon', dependencies = TRUE)"

# Copy the start.sh script to the container
COPY start.sh /start.sh

# Give execute permission to the script
RUN chmod +x /start.sh

ENTRYPOINT ["/start.sh"]