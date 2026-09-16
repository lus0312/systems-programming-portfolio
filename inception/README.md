# Inception — Docker 기반 다중 서비스 환경

## 어떤 과제인가요?

Inception은 여러 서비스를 Docker 컨테이너로 나누고, 이들을 연결해 하나의 웹 서비스 환경을 구성하는 42의 시스템 관리 과제입니다. 핵심은 서비스를 실행하는 데서 나아가, 이미지가 만들어지는 과정과 컨테이너의 실행·통신·데이터 저장 방식을 이해하는 것입니다.

이 프로젝트에서는 Nginx, WordPress, MariaDB를 중심으로 웹 환경을 만들고 Redis와 FTP를 추가했습니다. Dockerfile과 Compose 설정을 직접 작성하며 각 서비스가 어떤 역할을 맡고 어떻게 연결되는지 학습했습니다.

## 구성과 담당 내용

Debian Bookworm 기반 이미지의 패키지 설치, 서비스 설정, 초기화 스크립트와 Compose 구성을 직접 작성했습니다.

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

## 주요 구현

- 서비스별 Dockerfile과 초기화 스크립트 작성
- Compose의 `inception` 네트워크에 서비스 연결, 서비스 이름으로 통신
- MariaDB와 WordPress 데이터에 named volume 적용
- 비밀번호 파일은 Compose secrets로 전달하고 Git에서 제외
- WordPress 설치·사용자 생성·Redis 플러그인 설정을 초기화 스크립트로 구성
- Nginx의 자체 서명 인증서 생성 및 TLS 설정

## 문제 해결: 볼륨 이름과 호스트 디렉터리의 차이

처음에는 named volume을 사용하므로 데이터 저장 디렉터리를 삭제해도 다시 빌드하면 경로가 자동으로 만들어질 것으로 예상했습니다. 실제 설정은 `local` 드라이버의 `type: none`, `o: bind`, `device`로 특정 호스트 경로를 연결하는 방식이었습니다.

이 구성에서는 이미지 빌드와 별개로, 컨테이너 실행 시 마운트할 호스트 디렉터리가 있어야 합니다. 현재 Makefile은 `docker compose up` 전에 `mkdir -p`로 다음 두 디렉터리를 생성합니다.

```text
/home/eulee/data/mariadb
/home/eulee/data/wordpress
```

이 과정을 통해 이미지 빌드, 컨테이너 실행, Docker 볼륨 객체, 호스트 저장 경로가 서로 다른 역할을 갖는다는 점을 이해했습니다.

## 배운 점

### 이미지와 컨테이너의 역할

Dockerfile로 실행 환경을 이미지로 만들고, Compose로 그 이미지를 실행할 때의 네트워크·볼륨·설정을 정의했습니다. 이 과정을 통해 이미지를 만드는 단계와 컨테이너를 실행하는 단계를 구분하게 됐습니다.

### 서비스 분리와 통신

웹 요청 처리, PHP 실행, 데이터 저장, 캐시를 서로 다른 컨테이너로 나누었습니다. Compose 네트워크 안에서 서비스 이름으로 연결하면서, 컨테이너가 분리되어 있어도 명시적인 연결 설정으로 하나의 서비스를 구성할 수 있음을 배웠습니다.

### 컨테이너와 데이터의 수명

데이터 저장 실습과 호스트 경로 오류를 통해 컨테이너의 수명과 저장 데이터의 수명을 별도로 생각해야 한다는 점을 배웠습니다. 볼륨에 이름을 붙이는 것과 실제 데이터가 저장될 호스트 디렉터리를 준비하는 일도 구분하게 됐습니다.

### 학습 방법

도서로 Docker와 컨테이너의 기본 개념을 학습하고, AI를 활용한 실습·질의응답·퀴즈로 이해도를 점검했습니다. 실제 설정을 작성하고 데이터 저장 동작을 실습하면서 개념을 적용했습니다.

## 실행 준비

Linux 환경과 Docker Engine, Docker Compose, GNU Make가 필요합니다. 아래 명령은 이 프로젝트 폴더 기준입니다. 네트워크를 통한 패키지·WordPress·WP-CLI 다운로드가 발생합니다.

1. `cp srcs/.env.example srcs/.env`로 로컬 환경 파일을 만듭니다.
2. `secrets/`에 아래 파일을 만들고 각각 사용할 비밀번호를 한 줄로 넣습니다. 실제 값은 저장소에 올리지 않습니다.

```text
secrets/db_password.txt
secrets/wp_admin_password.txt
secrets/wp_user_password.txt
secrets/ftp_password.txt
```

3. `/home/eulee/data/` 하위 경로를 생성할 권한을 준비합니다. 저장 위치를 바꾸려면 Makefile의 `DATA_DIR`와 Compose의 두 `device` 경로를 함께 바꿉니다.
4. 기본 도메인 `eulee.42.fr`이 Docker 호스트를 가리키도록 로컬 이름 해석을 설정합니다. 도메인을 바꾸려면 `.env` 외에 Nginx 설정과 인증서 생성 스크립트도 함께 수정해야 합니다.
5. 실행합니다.

```bash
make
docker compose -f srcs/docker-compose.yml ps
docker compose -f srcs/docker-compose.yml logs --tail=100
```

기본 접속 주소는 `https://eulee.42.fr`이며 자체 서명 인증서를 사용합니다. 서비스 중지에는 `make down`을 사용합니다. 현재 Makefile의 `clean`도 컨테이너를 내리는 동작이며, 호스트 데이터 디렉터리를 삭제하지 않습니다.
