#include <SFML/Graphics.hpp>
#include <algorithm>
#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include "TableAdapters.h"
#include "PolynomFormat.h"

struct Button
{
    sf::RectangleShape box;
    sf::Text label;

    bool contains(const sf::Vector2f& p) const
    {
        return box.getGlobalBounds().contains(p);
    }

    void draw(sf::RenderTarget& target) const
    {
        target.draw(box);
        target.draw(label);
    }
};

struct TextBox
{
    sf::RectangleShape box;
    sf::Text text;
    std::string value;
    bool active = false;
    bool multiline = false;

    bool contains(const sf::Vector2f& p) const
    {
        return box.getGlobalBounds().contains(p);
    }

    void sync()
    {
        text.setString(value.empty() ? "" : value);
    }

    void draw(sf::RenderTarget& target) const
    {
        target.draw(box);
        target.draw(text);
    }
};

static sf::Color bg(246, 247, 251);
static sf::Color panel(255, 255, 255);
static sf::Color border(200, 205, 215);
static sf::Color accent(71, 110, 255);
static sf::Color softButton(245, 247, 251);
static sf::Color textDark(35, 41, 55);
static sf::Color textMuted(110, 118, 135);
static sf::Color danger(202, 62, 71);
static sf::Color ok(33, 156, 98);

Button makeButton(const sf::Font& font, float x, float y, float w, float h, const std::string& title, bool filled = false)
{
    Button b;
    b.box.setPosition(x, y);
    b.box.setSize({ w, h });
    b.box.setFillColor(softButton);
    b.box.setOutlineThickness(filled ? 2.f : 1.f);
    b.box.setOutlineColor(filled ? accent : border);

    b.label.setFont(font);
    b.label.setCharacterSize(16);
    b.label.setFillColor(textDark);
    b.label.setString(title);

    sf::FloatRect lb = b.label.getLocalBounds();
    b.label.setPosition(
        x + (w - lb.width) / 2.f - lb.left,
        y + (h - lb.height) / 2.f - lb.top - 2.f
    );
    return b;
}

TextBox makeTextBox(const sf::Font& font, float x, float y, float w, float h, bool multiline = false)
{
    TextBox tb;
    tb.box.setPosition(x, y);
    tb.box.setSize({ w, h });
    tb.box.setFillColor(sf::Color::White);
    tb.box.setOutlineThickness(1.f);
    tb.box.setOutlineColor(border);
    tb.text.setFont(font);
    tb.text.setCharacterSize(18);
    tb.text.setFillColor(textDark);
    tb.text.setPosition(x + 10.f, y + 8.f);
    tb.multiline = multiline;
    return tb;
}

void setActive(TextBox& tb, bool activeState)
{
    tb.active = activeState;
    tb.box.setOutlineColor(activeState ? accent : border);
    tb.box.setOutlineThickness(activeState ? 2.f : 1.f);
}

std::string shorten(const std::string& s, std::size_t maxLen)
{
    if (s.size() <= maxLen) return s;
    return s.substr(0, maxLen - 3) + "...";
}

std::string fitSingleLine(const sf::Font& font, const std::string& s, unsigned int size, float maxWidth)
{
    sf::Text probe;
    probe.setFont(font);
    probe.setCharacterSize(size);
    probe.setString(s);
    if (probe.getLocalBounds().width <= maxWidth) return s;

    std::string out = s;
    while (!out.empty())
    {
        out.pop_back();
        probe.setString(out + "...");
        if (probe.getLocalBounds().width <= maxWidth)
            return out + "...";
    }
    return "";
}

std::vector<std::string> wrapTextLines(const sf::Font& font, const std::string& s, unsigned int size, float maxWidth, std::size_t maxLines)
{
    std::vector<std::string> lines;
    std::istringstream iss(s);
    std::string word, current;
    sf::Text probe;
    probe.setFont(font);
    probe.setCharacterSize(size);

    while (iss >> word)
    {
        std::string candidate = current.empty() ? word : current + " " + word;
        probe.setString(candidate);
        if (probe.getLocalBounds().width <= maxWidth)
        {
            current = candidate;
        }
        else
        {
            if (!current.empty()) lines.push_back(current);
            current = word;
            if (lines.size() + 1 == maxLines)
            {
                lines.push_back(fitSingleLine(font, current + " " + std::string(std::istreambuf_iterator<char>(iss), {}), size, maxWidth));
                return lines;
            }
        }
    }
    if (!current.empty() && lines.size() < maxLines) lines.push_back(current);
    return lines;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(1400, 900), "Polynomial", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    sf::Font font;
    bool fontLoaded =
        font.loadFromFile("C:/Windows/Fonts/arial.ttf") ||
        font.loadFromFile("arial.ttf") ||
        font.loadFromFile("C:/Windows/Fonts/segoeui.ttf");

    if (!fontLoaded)
        return 1;

    auto storages = GetStorageItems();
    StorageType selectedType = StorageType::OrderedTable;
    std::unique_ptr<ITableAdapter> table = CreateAdapter(selectedType);

    TextBox keyBox = makeTextBox(font, 24, 110, 300, 42);
    TextBox polyBox = makeTextBox(font, 24, 184, 300, 42);
    TextBox p1Box = makeTextBox(font, 24, 586, 300, 42);
    TextBox p2Box = makeTextBox(font, 24, 660, 300, 42);

    Button addBtn = makeButton(font, 24, 248, 145, 42, "Add");
    Button findBtn = makeButton(font, 179, 248, 145, 42, "Find");
    Button delBtn = makeButton(font, 24, 300, 145, 42, "Delete");
    Button clearBtn = makeButton(font, 179, 300, 145, 42, "Clear");

    Button plusBtn = makeButton(font, 24, 724, 55, 42, "+");
    Button minusBtn = makeButton(font, 89, 724, 55, 42, "-");
    Button mulBtn = makeButton(font, 154, 724, 55, 42, "*");
    Button divBtn = makeButton(font, 219, 724, 55, 42, "/");
    Button modBtn = makeButton(font, 284, 724, 40, 42, "%");

    std::vector<Button> storageButtons;
    float sx = 360.f;
    float sy = 26.f;
    for (std::size_t i = 0; i < storages.size(); ++i)
    {
        float w = 128.f;
        if (i == 3) w = 150.f;
        storageButtons.push_back(makeButton(font, sx, sy, w, 40, storages[i].second, storages[i].first == selectedType));
        sx += w + 10.f;
    }

    std::string status = "Ready";
    sf::Color statusColor = textMuted;
    float recordsScroll = 0.f;

    auto refreshStorageButtons = [&]()
        {
            float x = 360.f;
            for (std::size_t i = 0; i < storageButtons.size(); ++i)
            {
                bool activeStorage = storages[i].first == selectedType;
                float w = storageButtons[i].box.getSize().x;
                storageButtons[i] = makeButton(font, x, 26.f, w, 40, storages[i].second, activeStorage);
                x += w + 10.f;
            }
        };

    auto setStatus = [&](const std::string& msg, sf::Color color = textMuted)
        {
            status = msg;
            statusColor = color;
        };

    auto parsePolynomFromBox = [&](const TextBox& box) -> polinom
        {
            return polinom(box.value);
        };

    auto resolveOperand = [&](const TextBox& box) -> polinom
        {
            polinom found;
            if (table->find(box.value, found))
                return found;
            return polinom(box.value);
        };

    auto executeSafe = [&](const std::function<void()>& fn)
        {
            try
            {
                fn();
            }
            catch (...)
            {
                setStatus("Operation failed. Check polynomial format or selected structure.", danger);
            }
        };

    auto records = [&]() { return table->all(); };

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseWheelScrolled)
            {
                recordsScroll -= event.mouseWheelScroll.delta * 28.f;
                if (recordsScroll < 0.f) recordsScroll = 0.f;
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mouse = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });

                setActive(keyBox, keyBox.contains(mouse));
                setActive(polyBox, polyBox.contains(mouse));
                setActive(p1Box, p1Box.contains(mouse));
                setActive(p2Box, p2Box.contains(mouse));

                for (std::size_t i = 0; i < storageButtons.size(); ++i)
                {
                    if (storageButtons[i].contains(mouse))
                    {
                        selectedType = storages[i].first;
                        table = CreateAdapter(selectedType);
                        refreshStorageButtons();
                        setStatus(std::string("Selected storage: ") + table->name(), accent);
                    }
                }

                if (addBtn.contains(mouse))
                {
                    executeSafe([&]()
                        {
                            if (keyBox.value.empty()) throw 1;
                            polinom p = parsePolynomFromBox(polyBox);
                            bool okInsert = table->insert(keyBox.value, p);
                            setStatus(okInsert ? "Inserted" : "Key already exists", okInsert ? ok : danger);
                        });
                }

                if (findBtn.contains(mouse))
                {
                    executeSafe([&]()
                        {
                            polinom found;
                            bool okFind = table->find(keyBox.value, found);
                            if (okFind)
                            {
                                polyBox.value = polinomToString(found);
                                polyBox.sync();
                                setStatus("Found: " + polyBox.value, ok);
                            }
                            else
                            {
                                setStatus("Key not found", danger);
                            }
                        });
                }

                if (delBtn.contains(mouse))
                {
                    bool removed = table->remove(keyBox.value);
                    setStatus(removed ? "Deleted" : "Key not found", removed ? ok : danger);
                }

                if (clearBtn.contains(mouse))
                {
                    table->clear();
                    setStatus("Storage cleared", ok);
                }

                if (plusBtn.contains(mouse))
                {
                    executeSafe([&]()
                        {
                            polinom res = resolveOperand(p1Box) + resolveOperand(p2Box);
                            setStatus("P1 + P2 = " + polinomToString(res), accent);
                        });
                }

                if (minusBtn.contains(mouse))
                {
                    executeSafe([&]()
                        {
                            polinom res = resolveOperand(p1Box) - resolveOperand(p2Box);
                            setStatus("P1 - P2 = " + polinomToString(res), accent);
                        });
                }

                if (mulBtn.contains(mouse))
                {
                    executeSafe([&]()
                        {
                            polinom res = resolveOperand(p1Box) * resolveOperand(p2Box);
                            setStatus("P1 * P2 = " + polinomToString(res), accent);
                        });
                }

                if (divBtn.contains(mouse))
                {
                    executeSafe([&]()
                        {
                            polinom res = resolveOperand(p1Box) / resolveOperand(p2Box);
                            setStatus("P1 / P2 = " + polinomToString(res), accent);
                        });
                }

                if (modBtn.contains(mouse))
                {
                    executeSafe([&]()
                        {
                            polinom res = resolveOperand(p1Box) % resolveOperand(p2Box);
                            setStatus("P1 % P2 = " + polinomToString(res), accent);
                        });
                }
            }

            if (event.type == sf::Event::TextEntered)
            {
                TextBox* activeBox = nullptr;
                if (keyBox.active) activeBox = &keyBox;
                else if (polyBox.active) activeBox = &polyBox;
                else if (p1Box.active) activeBox = &p1Box;
                else if (p2Box.active) activeBox = &p2Box;

                if (activeBox)
                {
                    unsigned int ch = event.text.unicode;
                    if (ch == 8)
                    {
                        if (!activeBox->value.empty())
                            activeBox->value.pop_back();
                    }
                    else if (ch == 13)
                    {
                    }
                    else if (ch >= 32 && ch < 127)
                    {
                        activeBox->value.push_back(static_cast<char>(ch));
                    }
                    activeBox->sync();
                }
            }
        }

        auto list = records();
        float maxScroll = std::max(0.f, static_cast<float>(list.size()) * 34.f - 620.f);
        if (recordsScroll > maxScroll) recordsScroll = maxScroll;

        window.clear(bg);

        sf::RectangleShape leftPanel({ 340.f, 852.f });
        leftPanel.setPosition(16.f, 16.f);
        leftPanel.setFillColor(panel);
        leftPanel.setOutlineColor(border);
        leftPanel.setOutlineThickness(1.f);
        window.draw(leftPanel);

        sf::RectangleShape rightPanel({ 1028.f, 852.f });
        rightPanel.setPosition(356.f, 16.f);
        rightPanel.setFillColor(panel);
        rightPanel.setOutlineColor(border);
        rightPanel.setOutlineThickness(1.f);
        window.draw(rightPanel);

        auto drawLabel = [&](const std::string& str, float x, float y, int size, sf::Color color = textDark, bool bold = false)
            {
                sf::Text t;
                t.setFont(font);
                t.setCharacterSize(size);
                t.setFillColor(color);
                t.setString(str);
                t.setPosition(x, y);
                window.draw(t);
            };

        drawLabel("Polynomial Manager", 24.f, 28.f, 26);
        drawLabel("Key", 24.f, 82.f, 15, textMuted);
        drawLabel("Polynomial", 24.f, 156.f, 15, textMuted);
        drawLabel("Table actions", 24.f, 225.f, 15, textMuted);
        drawLabel("Current storage", 24.f, 376.f, 15, textMuted);
        drawLabel(table->name(), 24.f, 402.f, 20, accent);
        drawLabel("Data structures", 360.f, 0.f, 14, textMuted);
        drawLabel("Operand 1 (key or polynomial)", 24.f, 558.f, 15, textMuted);
        drawLabel("Operand 2 (key or polynomial)", 24.f, 632.f, 15, textMuted);

        keyBox.draw(window);
        polyBox.draw(window);
        p1Box.draw(window);
        p2Box.draw(window);

        addBtn.draw(window);
        findBtn.draw(window);
        delBtn.draw(window);
        clearBtn.draw(window);
        plusBtn.draw(window);
        minusBtn.draw(window);
        mulBtn.draw(window);
        divBtn.draw(window);
        modBtn.draw(window);

        for (const auto& btn : storageButtons) btn.draw(window);

        sf::RectangleShape statusBar({ 308.f, 94.f });
        statusBar.setPosition(24.f, 452.f);
        statusBar.setFillColor(sf::Color(250, 251, 255));
        statusBar.setOutlineColor(border);
        statusBar.setOutlineThickness(1.f);
        window.draw(statusBar);

        drawLabel("Status", 36.f, 464.f, 15, textMuted);
        auto statusLines = wrapTextLines(font, status, 15, 280.f, 3);
        for (std::size_t i = 0; i < statusLines.size(); ++i)
        {
            sf::Text statusText;
            statusText.setFont(font);
            statusText.setCharacterSize(15);
            statusText.setString(statusLines[i]);
            statusText.setFillColor(statusColor);
            statusText.setPosition(36.f, 490.f + static_cast<float>(i) * 18.f);
            window.draw(statusText);
        }

        drawLabel("Records", 380.f, 78.f, 22);
        drawLabel("Key", 390.f, 116.f, 15, textMuted);
        drawLabel("Polynomial", 620.f, 116.f, 15, textMuted);

        sf::RectangleShape headerLine({ 980.f, 1.f });
        headerLine.setPosition(380.f, 144.f);
        headerLine.setFillColor(border);
        window.draw(headerLine);

        float startY = 160.f - recordsScroll;
        for (std::size_t i = 0; i < list.size(); ++i)
        {
            float y = startY + static_cast<float>(i) * 34.f;
            if (y < 148.f || y > 820.f) continue;

            sf::RectangleShape rowBg({ 980.f, 30.f });
            rowBg.setPosition(380.f, y);
            rowBg.setFillColor(i % 2 == 0 ? sf::Color(252, 253, 255) : sf::Color::White);
            window.draw(rowBg);

            drawLabel(list[i].key, 390.f, y + 4.f, 16);
            drawLabel(fitSingleLine(font, polinomToString(list[i].value), 16, 700.f), 620.f, y + 4.f, 16);

            sf::RectangleShape rowLine({ 980.f, 1.f });
            rowLine.setPosition(380.f, y + 30.f);
            rowLine.setFillColor(sf::Color(238, 241, 246));
            window.draw(rowLine);
        }


        window.display();
    }
    return 0;
}
