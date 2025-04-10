# Adapter Pattern

- **구조적(Structural) 디자인 패턴** 중 하나로, 서로 다른 인터페이스를 가진 클래스들이 함께 동작할 수 있도록 연결(적응)하는 패턴
- 기존 클래스의 인터페이스를 클라이언트가 기대하는 인터페이스로 변환하여 **호환성 문제를 해결**함

    > 어댑터는 두 개의 독립적이고 호환되지 않는 인터페이스 간의 다리를 제공

### 구성 요소

1. **Target (목표 인터페이스)**  
   - 클라이언트가 기대하는 인터페이스  
   - 어댑터가 변환해야 할 대상 인터페이스  

2. **Adaptee (적응 대상)**  
   - 기존 클래스 또는 호환되지 않는 인터페이스를 가진 클래스  

3. **Adapter (어댑터)**  
   - `Target` 인터페이스를 구현하고, 내부적으로 `Adaptee`를 호출하여 변환 작업 수행  

4. **Client (클라이언트)**  
   - `Target` 인터페이스를 사용하며, 내부적으로 어댑터를 통해 `Adaptee`와 상호작용  

---

### 사용 이유

- 기존 코드나 라이브러리를 수정하지 않고 새로운 인터페이스와 통합하기 위해
- 서로 다른 시스템 간의 호환성을 유지하면서 재사용성을 높이기 위해
- 클라이언트와 구현된 인터페이스 간의 결합도를 낮추기 위해

---

### 주로 사용하는 경우

- 기존 클래스가 클라이언트에서 요구하는 인터페이스와 맞지 않는 경우
    > 예: XML 데이터를 JSON으로 변환, 서로 다른 전압을 사용하는 장치 연결 등
- 외부 라이브러리나 서드파티 코드와 통합해야 하는 경우

---

### 장점

- **재사용성 증가**: 기존 클래스를 수정하지 않고 새로운 환경에서 재사용 가능
- **유연성**: 클라이언트는 어댑터를 통해 다양한 구현체와 상호작용 가능
- **결합도 감소**: 클라이언트와 적응 대상 클래스 간의 직접적인 의존성 제거

---

### 단점

- 어댑터 클래스가 많아질 경우 코드 복잡도가 증가할 수 있음
- 잘못된 사용 시 설계가 지나치게 복잡해질 위험 존재

---

### 예제 코드

```Java
// Target Interface
public interface MediaPlayer {
    void play(String audioType, String fileName);
}

// Adaptee Class
public interface AdvancedMediaPlayer {
    void playVlc(String fileName);
    void playMp4(String fileName);
}

public class VlcPlayer implements AdvancedMediaPlayer {
    @Override
    public void playVlc(String fileName) {
        System.out.println("Playing vlc file: " + fileName);
    }

    @Override
    public void playMp4(String fileName) {
        // Do nothing
    }
}

public class Mp4Player implements AdvancedMediaPlayer {
    @Override
    public void playVlc(String fileName) {
        // Do nothing
    }

    @Override
    public void playMp4(String fileName) {
        System.out.println("Playing mp4 file: " + fileName);
    }
}

// Adapter Class
public class MediaAdapter implements MediaPlayer {
    private AdvancedMediaPlayer advancedMediaPlayer;

    public MediaAdapter(String audioType) {
        if (audioType.equalsIgnoreCase("vlc")) {
            advancedMediaPlayer = new VlcPlayer();
        } else if (audioType.equalsIgnoreCase("mp4")) {
            advancedMediaPlayer = new Mp4Player();
        }
    }

    @Override
    public void play(String audioType, String fileName) {
        if (audioType.equalsIgnoreCase("vlc")) {
            advancedMediaPlayer.playVlc(fileName);
        } else if (audioType.equalsIgnoreCase("mp4")) {
            advancedMediaPlayer.playMp4(fileName);
        }
    }
}

// Client Class
public class AudioPlayer implements MediaPlayer {
    private MediaAdapter mediaAdapter;

    @Override
    public void play(String audioType, String fileName) {
        if (audioType.equalsIgnoreCase("mp3")) {
            System.out.println("Playing mp3 file: " + fileName);
        } else if (audioType.equalsIgnoreCase("vlc") || audioType.equalsIgnoreCase("mp4")) {
            mediaAdapter = new MediaAdapter(audioType);
            mediaAdapter.play(audioType, fileName);
        } else {
            System.out.println("Invalid media type: " + audioType);
        }
    }
}

// Main Class
public class AdapterPatternDemo {
    public static void main(String[] args) {
        AudioPlayer audioPlayer = new AudioPlayer();
        
        audioPlayer.play("mp3", "song.mp3");
        audioPlayer.play("vlc", "movie.vlc");
        audioPlayer.play("mp4", "video.mp4");
        audioPlayer.play("avi", "unsupported.avi");
    }
}
```

---

### 실행 결과

```
Playing mp3 file: song.mp3  
Playing vlc file: movie.vlc  
Playing mp4 file: video.mp4  
Invalid media type: avi  
```

---

### 활용 사례

- **전압 변환기**: 서로 다른 전압을 사용하는 기기 연결 (예: 모바일 충전기)
- **데이터 형식 변환**: XML 데이터를 JSON으로 변환하여 사용
- **소프트웨어 통합**: 서드파티 라이브러리와 기존 시스템 간의 호환성 유지

---

**Adapter 패턴은 기존 코드 수정 없이 새로운 환경에 적응할 수 있도록 설계된 강력한 도구지만, 불필요한 복잡성을 피하기 위해 필요한 경우에만 사용하는 것이 좋다.**
