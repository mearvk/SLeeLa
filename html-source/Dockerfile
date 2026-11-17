# Build context: this directory (html-source/).  docker build -t sleela-site .
FROM maven:3.9-eclipse-temurin-21 AS build
WORKDIR /src
COPY pom.xml .
RUN mvn -q -B -Dnobundle dependency:go-offline
COPY src src
RUN mvn -q -B -Dnobundle package

FROM tomcat:10.1-jre21-temurin
RUN rm -rf /usr/local/tomcat/webapps/* \
 && sed -i 's|<Connector port="8080" protocol="HTTP/1.1"|<Connector port="8080" protocol="HTTP/1.1" compression="on" compressibleMimeType="text/html,text/css,application/javascript,image/svg+xml,text/plain,application/json" server="sleela"|' /usr/local/tomcat/conf/server.xml \
 && grep -q 'compression="on"' /usr/local/tomcat/conf/server.xml \
 && mkdir -p /data/sleela \
 && chown -R 1000:1000 /usr/local/tomcat/logs /usr/local/tomcat/work /usr/local/tomcat/temp /usr/local/tomcat/webapps
COPY --from=build --chown=1000:1000 /src/target/sleela-site.war /usr/local/tomcat/webapps/ROOT.war
# Mount a checkout of mearvk/SLeeLa here (read-only is fine).
ENV SLEELA_HOME=/data/sleela
USER 1000:1000
EXPOSE 8080
HEALTHCHECK --interval=30s --timeout=5s --start-period=40s CMD ["bash","-c","exec 3<>/dev/tcp/127.0.0.1/8080 && printf 'GET /healthz HTTP/1.0\\r\\n\\r\\n' >&3 && head -n1 <&3 | grep -q ' 200 '"]
