ARG  from
FROM ${from}

WORKDIR /usr/src/PyPoc

COPY script/ script/

RUN DEBIAN_FRONTEND=noninteractive script/bootstrap 1
