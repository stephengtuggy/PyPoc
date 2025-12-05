ARG  from
FROM ${from}

WORKDIR /usr/local/src/PyPoc

COPY . .

ENTRYPOINT ["script/docker-entrypoint.sh"]
