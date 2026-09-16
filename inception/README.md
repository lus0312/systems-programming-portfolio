# Inception — Docker 기반 다중 서비스 환경

여러 서비스를 Docker 컨테이너로 분리하고 하나의 웹 서비스 환경으로 연결한 42 시스템 관리 과제입니다. 공개 이미지를 가져다 쓰는 대신 Debian Bookworm 기반 이미지를 직접 작성해, 이미지가 만들어지는 과정과 컨테이너의 실행 · 통신 · 데이터 저장 방식을 다뤘습니다.

| 항목 | 내용 |
| --- | --- |
| 인원 | 개인 과제 |
| 환경 · 도구 | Docker Engine, Docker Compose, Debian Bookworm, Nginx, MariaDB, Redis, vsftpd, GNU Make |
| 담당 | 이미지 작성, 서비스 설정, 초기화 스크립트, Compose 구성 전 범위 |

## 설계

| 서비스 | 역할 | 연결 |
| --- | --- | --- |
| Nginx | HTTPS 요청 수신, 정적 파일 제공, PHP 요청 전달 | 호스트 443 → WordPress:9000 |
| WordPress / PHP-FPM | 웹 애플리케이션 실행 | MariaDB, Redis |
| MariaDB | WordPress 데이터베이스 | 내부 네트워크 |
| Redis | WordPress 객체 캐시 | 내부 네트워크 |
| FTP / vsftpd | WordPress 파일 접근 | 호스트 21, 21100–21110 |

```text
브라우저 → Nginx → WordPress / PHP-FPM → MariaDB
                                └──→ Redis

FTP 클라이언트 → FTP ──┐
WordPress ──────────────┼→ wordpress_data
Nginx (읽기 전용) ─────┘
```

## 문제 해결

### 볼륨 이름과 실제 호스트 저장 경로의 차이

처음에는 named volume을 사용하므로 데이터 저장 디렉터리를 삭제해도 다시 빌드하면 경로가 자동으로 생성될 것으로 예상했습니다. 하지만 실제 설정은 `local` 드라이버의 `type: none`, `o: bind`, `device`로 **특정 호스트 경로를 연결하는 방식**이었고, 이 구성에서는 컨테이너 실행 시점에 마운트할 호스트 디렉터리가 이미 존재해야 합니다.

Makefile에서 `docker compose up` 이전에 `mkdir -p`로 데이터 디렉터리를 생성하도록 구성해 해결했습니다.

```text
${DATA_DIR}/mariadb
${DATA_DIR}/wordpress
```

이 과정을 통해 **이미지 빌드 / 컨테이너 실행 / Docker 볼륨 객체 / 호스트 저장 경로**가 각각 다른 역할을 가진다는 점을 구분하게 됐습니다. 볼륨에 이름을 붙이는 일과 데이터가 실제로 저장될 위치를 준비하는 일은 별개의 작업입니다.

### 비밀 정보를 이미지와 저장소에서 분리

데이터베이스와 관리자 계정의 비밀번호를 Dockerfile이나 환경 변수 파일에 직접 두지 않고, Compose secrets로 컨테이너에 전달하도록 구성했습니다. 비밀번호 파일과 `.env`는 저장소에서 제외하고 예시 파일만 포함했습니다.

## 주요 구현

- 서비스별 Dockerfile과 초기화 스크립트 작성 (Debian Bookworm 기반)
- Compose의 `inception` 네트워크에 서비스를 연결하고 서비스 이름으로 통신
- MariaDB와 WordPress 데이터에 named volume 적용
- 비밀번호는 Compose secrets로 전달하고 Git에서 제외
- WordPress 설치 · 사용자 생성 · Redis 플러그인 설정을 초기화 스크립트로 자동화
- Nginx 자체 서명 인증서 생성 및 TLS 설정

## 배운 점

**이미지와 컨테이너의 역할** — Dockerfile로 실행 환경을 이미지로 만들고, Compose로 그 이미지를 실행할 때의 네트워크 · 볼륨 · 설정을 정의했습니다. 빌드 시점에 결정되는 것과 실행 시점에 주입되는 것을 구분하게 됐습니다.

**서비스 분리와 통신** — 웹 요청 처리, PHP 실행, 데이터 저장, 캐시를 서로 다른 컨테이너로 나누었습니다. 컨테이너가 분리되어 있어도 명시적인 네트워크 · 포트 설정으로 하나의 서비스를 구성할 수 있음을 확인했습니다.

**컨테이너와 데이터의 수명** — 컨테이너를 내리고 다시 올려도 데이터가 유지되어야 하므로, 컨테이너의 수명과 저장 데이터의 수명을 별도로 설계해야 한다는 점을 다뤘습니다.

## 실행 방법

Linux 환경과 Docker Engine, Docker Compose, GNU Make가 필요합니다. 아래 명령은 이 프로젝트 폴더 기준이며, 패키지 · WordPress · WP-CLI 다운로드를 위해 네트워크 연결이 필요합니다.

1. 환경 파일을 만듭니다.

```bash
cp srcs/.env.example srcs/.env
```

2. `secrets/`에 아래 파일을 만들고 사용할 비밀번호를 한 줄씩 넣습니다. 실제 값은 저장소에 포함하지 않습니다.

```text
secrets/db_password.txt
secrets/wp_admin_password.txt
secrets/wp_user_password.txt
secrets/ftp_password.txt
```

3. 데이터 디렉터리를 생성할 권한을 준비합니다. 저장 위치를 바꾸려면 Makefile의 `DATA_DIR`와 Compose의 두 `device` 경로를 함께 수정합니다.

4. 기본 도메인이 Docker 호스트를 가리키도록 로컬 이름 해석(`/etc/hosts`)을 설정합니다. 도메인을 변경하려면 `.env` 외에 Nginx 설정과 인증서 생성 스크립트도 함께 수정해야 합니다.

5. 실행하고 상태를 확인합니다.

```bash
make
docker compose -f srcs/docker-compose.yml ps
docker compose -f srcs/docker-compose.yml logs --tail=100
```

접속 주소는 `.env`에 설정한 도메인이며 자체 서명 인증서를 사용합니다. 서비스 중지는 `make down`을 사용합니다. `make clean`도 컨테이너를 내리는 동작이며 호스트 데이터 디렉터리는 삭제하지 않습니다.
