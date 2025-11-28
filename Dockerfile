ARG  from
FROM ${from}

WORKDIR /usr/src/PyPoc

COPY . .

ENTRYPOINT ["script/docker-entrypoint.sh"]
