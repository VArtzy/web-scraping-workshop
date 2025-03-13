import * as cheerio from 'cheerio'

const url = 'https://www.example.com'

try {
    const response = await fetch(url)

    const $ = cheerio.load(await response.text())
    const title = $('h1').text()
    const text = $('p').text()
    const link = $('a').attr('href')
    console.log(title)
    console.log(text)
    console.log(link)
} catch {
    console.log("check your internet connection")
}
//} catch (e) {
//    console.log(e)
//}
