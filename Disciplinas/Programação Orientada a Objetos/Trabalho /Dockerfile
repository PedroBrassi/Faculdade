# US-2381 - Entrega do sistema acadêmico via Docker.
#
# Build multi-stage: compila com Maven + JDK 21 e roda apenas com o JRE.
# A imagem executa a interface de linha de comando (CLI). Para interagir,
# rode com TTY:  docker run -it academic-system

# ---- Build stage ----
FROM maven:3.9-eclipse-temurin-21 AS build
WORKDIR /app

# Cache de dependências
COPY pom.xml .
RUN mvn -q -e -B dependency:go-offline

# Código-fonte e empacotamento (fat-jar via maven-shade-plugin)
COPY src ./src
RUN mvn -q -e -B clean package -DskipTests

# ---- Runtime stage ----
FROM eclipse-temurin:21-jre
WORKDIR /app
COPY --from=build /app/target/academic-system-1.0-SNAPSHOT.jar app.jar

# Main class do jar shaded = CLI (org.example.academic.system.Main)
ENTRYPOINT ["java", "-jar", "app.jar"]
